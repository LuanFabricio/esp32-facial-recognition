// Copyright 2020-2021 Espressif Systems (Shanghai) PTE LTD
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_console.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_main.h"
#include "esp_cli.h"
#include "esp_timer.h"

#define IMAGE_COUNT 10
static float image_database[IMAGE_COUNT][256];


extern const uint8_t image_start0[]   asm("_binary_image0_start");
extern const uint8_t image_start1[]   asm("_binary_image1_start");
extern const uint8_t image_start2[]   asm("_binary_image2_start");
extern const uint8_t image_start3[]   asm("_binary_image3_start");
extern const uint8_t image_start4[]   asm("_binary_image4_start");
extern const uint8_t image_start5[]   asm("_binary_image5_start");
extern const uint8_t image_start6[]   asm("_binary_image6_start");
extern const uint8_t image_start7[]   asm("_binary_image7_start");
extern const uint8_t image_start8[]   asm("_binary_image8_start");
extern const uint8_t image_start9[]   asm("_binary_image9_start");

// static float image_features[10][255]= {0};
static const char *TAG = "[esp_cli]";

static int task_benchmark(int argc, char *argv[])
{
  printf("\n");
  int runs = 10;

  if (argc == 2) {
    runs = atoi(argv[1]);
  }

  if (runs > 1024) {
    ESP_LOGW(TAG, "The run(%i) > MAX (1024)", runs);
  }

  float inference_time[1024] = {0};
  double inference_time_avg = 0.0f;
  unsigned detect_time, inference_time_index;
  for (int i = 0; i < runs; i++) {
    double run_inference_time_avg = 0.0f;
    for (int j = 0; j < IMAGE_COUNT; j++) {
      inference_time_index = IMAGE_COUNT*i+j;
      detect_time = esp_timer_get_time();
      run_inference((void *)image_database[j]);
      inference_time[inference_time_index] = (esp_timer_get_time() - detect_time)/1000.0;
      ESP_LOGI(
          TAG,
          "Run %i, image %i delta time: %0.4lf\n",
          i, j, inference_time[inference_time_index]);
      run_inference_time_avg += inference_time[inference_time_index];
    }
    inference_time_avg += run_inference_time_avg;

    run_inference_time_avg /= IMAGE_COUNT;
    ESP_LOGI(TAG, "Run %i Avg inference time: %0.4lf\n", i, run_inference_time_avg);
  }

  inference_time_avg /= runs * IMAGE_COUNT;
  ESP_LOGI(TAG, "Avg. inference time: %0.4lf\n", inference_time_avg);

  return 0;
}

static int task_dump_cli_handler(int argc, char *argv[])
{
  int num_of_tasks = uxTaskGetNumberOfTasks();
  TaskStatus_t *task_array = calloc(1, num_of_tasks * sizeof(TaskStatus_t));
  /* Just to go to the next line */
  printf("\n");
  if (!task_array) {
    ESP_LOGE(TAG, "Memory not allocated for task list.");
    return 0;
  }
  num_of_tasks = uxTaskGetSystemState(task_array, num_of_tasks, NULL);
  printf("\tName\tNumber\tPriority\tStackWaterMark\n");
  for (int i = 0; i < num_of_tasks; i++) {
    printf("%16s\t%u\t%u\t%u\n",
        task_array[i].pcTaskName,
        (unsigned) task_array[i].xTaskNumber,
        (unsigned) task_array[i].uxCurrentPriority,
        (unsigned) task_array[i].usStackHighWaterMark);
  }
  free(task_array);
  return 0;
}

static int cpu_dump_cli_handler(int argc, char *argv[])
{
  /* Just to go to the next line */
  printf("\n");
#ifndef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
  printf("%s: To use this utility enable: Component config --> FreeRTOS --> Enable FreeRTOS to collect run time stats\n", TAG);
#else
  char *buf = calloc(1, 2 * 1024);
  vTaskGetRunTimeStats(buf);
  printf("%s: Run Time Stats:\n%s\n", TAG, buf);
  free(buf);
#endif
  return 0;
}

static int mem_dump_cli_handler(int argc, char *argv[])
{
  size_t free_internal = heap_caps_get_free_size(MALLOC_CAP_8BIT) - heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  size_t free_spram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

  size_t total_internal = heap_caps_get_total_size(MALLOC_CAP_8BIT) - heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  size_t total_spram = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);

  /* Just to go to the next line */
  printf("\n");
  printf("\tDescription\tInternal\tSPIRAM\n");
  printf("Current Free Memory\t%d\t\t%d\n", free_internal, free_spram);
  printf("Usage Memory\t\t%d\t\t%d\n", total_internal - free_internal, total_spram - free_spram);
  printf("Total Memory\t\t%d\t\t%d\n", total_internal, total_spram);
  printf("Largest Free Block\t%d\t\t%d\n",
      heap_caps_get_largest_free_block(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
      heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
  printf("Min. Ever Free Size\t%d\t\t%d\n",
      heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
      heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
  return 0;
}

static int inference_cli_handler(int argc, char *argv[])
{
  /* Just to go to the next line */
  printf("\n");
  if (argc != 2) {
    printf("%s: Incorrect arguments\n", TAG);
    return 0;
  }
  int image_number = atoi(argv[1]);

  if((image_number < 0) || (image_number >= IMAGE_COUNT)) {
    ESP_LOGE(TAG, "Please Enter a valid Number ( 0 - %d)", IMAGE_COUNT-1);
    return -1;
  }

  ESP_LOGI(TAG,"Running inference...");
  // char file_name[30];
  // sprintf(file_name, "image%d.raw", image_number);
  unsigned detect_time;
  detect_time = esp_timer_get_time();
  run_inference((void *)image_database[image_number]);
  detect_time = (esp_timer_get_time() - detect_time)/1000;
  ESP_LOGI(TAG,"Time required for the inference is %u ms", detect_time);

  mem_dump_cli_handler(0, 0);

  return 0;
}

static esp_console_cmd_t diag_cmds[] = {
  {
    .command = "mem-dump",
    .help = "",
    .func = mem_dump_cli_handler,
  },
  {
    .command = "task-dump",
    .help = "",
    .func = task_dump_cli_handler,
  },
  {
    .command = "cpu-dump",
    .help = "",
    .func = cpu_dump_cli_handler,
  },
  {
    .command = "detect_image",
    .help = "detect_image <image_number>"
      "Note: image numbers ranging from 0 - 9 only are valid",
    .func = inference_cli_handler,
  },
  {
    .command = "benchmark",
    .help = "benchmark [<amount_of_runs>]"
      "Note: The max amount of runs is 1024",
    .func = task_benchmark,
  }
};

int esp_cli_register_cmds()
{
  int cmds_num = sizeof(diag_cmds) / sizeof(esp_console_cmd_t);
  int i;
  for (i = 0; i < cmds_num; i++) {
    ESP_LOGI(TAG, "Registering command: %s", diag_cmds[i].command);
    esp_console_cmd_register(&diag_cmds[i]);
  }
  return 0;
}

#define IMAGE_DATABASE_INIT_X(x) \
    uint8_t* image##x = (uint8_t *) image_start##x;        \
    for (uint16_t j = 0; j < 256; j++) {              \
      image_database[x][j] = (image##x[j] - 127.5) / 128;\
    }                                                 \

static void image_database_init()
{
  IMAGE_DATABASE_INIT_X(0);
  IMAGE_DATABASE_INIT_X(1);
  IMAGE_DATABASE_INIT_X(2);
  IMAGE_DATABASE_INIT_X(3);
  IMAGE_DATABASE_INIT_X(4);
  IMAGE_DATABASE_INIT_X(5);
  IMAGE_DATABASE_INIT_X(6);
  IMAGE_DATABASE_INIT_X(7);
  IMAGE_DATABASE_INIT_X(8);
  IMAGE_DATABASE_INIT_X(9);
  // image_database[1] = (uint8_t *) image1_start;
  // image_database[2] = (uint8_t *) image2_start;
  // image_database[3] = (uint8_t *) image3_start;
  // image_database[4] = (uint8_t *) image4_start;
  // image_database[5] = (uint8_t *) image5_start;
  // image_database[6] = (uint8_t *) image6_start;
  // image_database[7] = (uint8_t *) image7_start;
  // image_database[8] = (uint8_t *) image8_start;
  // image_database[9] = (uint8_t *) image9_start;
}

int esp_cli_start()
{
  image_database_init();
  static int cli_started;
  if (cli_started) {
    return 0;
  }

  esp_console_repl_t *repl = NULL;
  esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();

  esp_console_register_help_command();
  esp_cli_register_cmds();
#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
  esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
  esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
  esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

#else
#error Unsupported console type
#endif
  ESP_ERROR_CHECK(esp_console_start_repl(repl));
  cli_started = 1;
  return 0;
}
