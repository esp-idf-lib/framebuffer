/**
 * Simple framebuffer example.
 *
 * The example fills an 8x8 RGB framebuffer with a moving HSV gradient and
 * renders it. There is no real display here, so the renderer callback simply
 * prints the first row of every frame.
 */

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>
#include <framebuffer.h>

#define WIDTH  8
#define HEIGHT 8

static const char *TAG = "example";
static framebuffer_t fb;

static esp_err_t render(framebuffer_t *fb, void *arg)
{
    const rgb_t *row = fb->data;
    char buf[WIDTH * 7 + 1];
    size_t pos = 0;

    for (size_t x = 0; x < fb->width; x++)
        pos += snprintf(buf + pos, sizeof(buf) - pos, " %02x%02x%02x",
                        row[x].r, row[x].g, row[x].b);

    ESP_LOGI(TAG, "frame %u:%s", (unsigned)fb->frame_num, buf);

    return ESP_OK;
}

static void task(void *pvParameters)
{
    ESP_ERROR_CHECK(fb_init(&fb, WIDTH, HEIGHT, render));

    uint8_t hue = 0;

    while (1)
    {
        ESP_ERROR_CHECK(fb_begin(&fb));

        for (size_t y = 0; y < fb.height; y++)
        {
            for (size_t x = 0; x < fb.width; x++)
            {
                hsv_t color =
                {
                    .h = hue + x * 8 + y * 16,
                    .s = 255,
                    .v = 255 - y * 16,
                };
                ESP_ERROR_CHECK(fb_set_pixel_hsv(&fb, x, y, color));
            }
        }

        ESP_ERROR_CHECK(fb_end(&fb));
        ESP_ERROR_CHECK(fb_render(&fb, NULL));

        hue += 4;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    xTaskCreate(task, TAG, configMINIMAL_STACK_SIZE * 4, NULL, 5, NULL);
}
