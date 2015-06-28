CMSIS的缩写为: Cortex Microcontroller Software Interface Standard，是ARM Cortex微控制器
软件接口标准，是ARM公司为芯片厂商提供的一套通用的且独立于芯片厂商
的处理器软件接口

stm32f10x_conf.h      //对STM32外设的配置头文件
stm32f10x.h           //内部硬件资源寄存器地址定义头文件
core_cm3.c            //是CMSIS Cortex-M3核外设接入层的源文件，在所有符合CMSIS 标准的Cortex-M3核系列单片机都适用，独立于芯片制造商，由ARM公司提供。
system_stm32f10x.c    //文件只是设置了系统时钟和总线时钟，至于那些外设的时钟是在rcc.c这个文件中实现的。