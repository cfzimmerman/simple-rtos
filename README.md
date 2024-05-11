# FreeRTOS Cortex MPS2 QEMU notification blinky

[YouTube Demo](https://youtu.be/QJrnPchQvNw)

In this project, I built, modified, and ran a `C` FreeRTOS app for ARM Cortex MPS2 on QEMU. Code-level changes were minimal, but the project required setting up cross compilation from my mac, configuring my clangd lsp in neovim, and rewriting the [Queue](https://www.freertos.org/Embedded-RTOS-Queues.html)-based demo application to use [Notifications](https://www.freertos.org/RTOS-task-notifications.html) instead. Doing so was just for the purpose of exploring the API. I also experimented with setting up the FreeRtos-Plus TCP demo but quickly reached the limits of my network virtualization knowledge.

The demo project can be found [here](https://github.com/cfzimmerman/simple-rtos/tree/main/FreeRTOS/Demo/CORTEX_MPS2_QEMU_IAR_GCC).
The code I added is in [main_blinky.c](https://github.com/cfzimmerman/simple-rtos/blob/main/FreeRTOS/Demo/CORTEX_MPS2_QEMU_IAR_GCC/main_blinky.c).
