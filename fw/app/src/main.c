#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/i2s.h>
#include <math.h>
#include <string.h>

#define LED0  DT_ALIAS_LED0_GPIOS_CONTROLLER
#define PIN   DT_ALIAS_LED0_GPIOS_PIN
#define FLAGS DT_ALIAS_LED0_GPIOS_FLAGS


static void blink_led_thread(void *unused1, void *unused2, void *unused3)
{
	ARG_UNUSED(unused1);
	ARG_UNUSED(unused2);
	ARG_UNUSED(unused3);

	int ret;
	struct device *dev;

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	while (1) {
		gpio_pin_set(dev, PIN, !gpio_pin_get(dev, PIN));
		k_sleep(500);
	}
}

#define PI 3.14159265
static void create_sine_wave(void* mem_block, int num_chans, int bits, int samples_per_cycle)
{
	ARG_UNUSED(bits);

	int i,n;
	double sample;
	u8_t *buf = mem_block;

	for (i = 0; i < samples_per_cycle; i++) {
		sample = sin(i * PI / 180.0);
		sample *= pow(2, bits - 1) - 1;

		for (n = 0; n < num_chans; n++) {
			//buf[3*(num_chans*i+n)+0] = (u8_t)(((int)sample) >> 16);
			buf[2*(num_chans*i+n)+0] = (u8_t)(((int)sample) >>  8);
			buf[2*(num_chans*i+n)+1] = (u8_t)(((int)sample) >>  0);
		}
	}
}

#define SAMPLE_RATE     (10000)
#define WAVE_FREQ_HZ    (500)
#define NUM_CHANS       (2)
#define WORD_SIZE       (16)
#define SAMPLE_PER_CYCLE (SAMPLE_RATE / WAVE_FREQ_HZ)

K_MEM_SLAB_DEFINE(mem_slab, NUM_CHANS * SAMPLE_PER_CYCLE * WORD_SIZE / 8, 10, 4);

static void sound_thread(void *unused1, void *unused2, void *unused3)
{
	ARG_UNUSED(unused1);
	ARG_UNUSED(unused2);
	ARG_UNUSED(unused3);

	int ret;
	struct device *dev;
	struct i2s_config cfg;
	void *mem_block_sine;
	void *mem_block;

	dev = device_get_binding(DT_ALIAS_I2S_LABEL);
	if (dev == NULL) {
		return;
	}

	/* configure i2s for audio playback */
	cfg.word_size = WORD_SIZE;
	cfg.channels  = NUM_CHANS;
	cfg.format    = I2S_FMT_DATA_FORMAT_I2S | I2S_FMT_CLK_NF_NB;
	cfg.options = I2S_OPT_FRAME_CLK_MASTER | I2S_OPT_BIT_CLK_MASTER;
	cfg.frame_clk_freq = SAMPLE_RATE;
	cfg.block_size = NUM_CHANS * SAMPLE_PER_CYCLE * WORD_SIZE / 8;
	cfg.mem_slab = &mem_slab;
	cfg.timeout = K_FOREVER;

	ret = i2s_configure(dev, I2S_DIR_TX, &cfg);
	if (ret < 0) {
	 	return;
	}

	ret = k_mem_slab_alloc(&mem_slab, &mem_block_sine, 1000);
	if (ret < 0) {
		return;
	}
	create_sine_wave(mem_block_sine, NUM_CHANS, WORD_SIZE, SAMPLE_PER_CYCLE);

	ret = k_mem_slab_alloc(&mem_slab, &mem_block, 1000);
	if (ret < 0) {
		return;
	}
	memcpy(mem_block, mem_block_sine, cfg.block_size);

	ret = i2s_write(dev, mem_block, NUM_CHANS * SAMPLE_PER_CYCLE * WORD_SIZE / 8);
	if (ret < 0) {
		while (1);
		return;
	}
	ret = i2s_trigger(dev, I2S_DIR_TX, I2S_TRIGGER_START);
	if (ret < 0) {
		while (1);
		return;
	}

	while (1) {
		ret = k_mem_slab_alloc(&mem_slab, &mem_block, 1000);
		if (ret < 0) {
			while (1);
			return;
		}
		memcpy(mem_block, mem_block_sine, cfg.block_size);

		ret = i2s_write(dev, mem_block, NUM_CHANS * SAMPLE_PER_CYCLE * WORD_SIZE / 8);
		if (ret < 0) {
			while (1);
			return;
		}
	}
}


K_THREAD_DEFINE(blink_led_thread_tid, 512,
	blink_led_thread, NULL, NULL, NULL,
	5, 0, K_NO_WAIT);

K_THREAD_DEFINE(sound_thread_tid, 4096,
	sound_thread, NULL, NULL, NULL,
	5, 0, K_NO_WAIT);
