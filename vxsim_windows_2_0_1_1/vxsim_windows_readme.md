## VxSim Windows  - vxsim VxWorks&reg; Board Support Package (BSP)  

## Supported Boards

The **vxsim** board support package (BSP) can be used to boot VxWorks on the **VxSim Windows**.


## Supported Devices

| Hardware Interface     | VxBus Driver Name | VxWorks Driver Component | VxBus Driver Module Source File |
| ---------------------- | ---------------------- | ----------- | ----------- |
| Standard Input/Output  |  |  |  |
| Virtual Disk  |  | INCLUDE_VIRTUAL_DISK | virtualDiskLib.c |
| Host Serial Input/Output driver   | vxsimHostSio | INCLUDE_HOST_SIO | vxbVxsimHostSio.c |
| Timer  |  |  |  |
| File System  | | INCLUDE_PASSFS | passfsLib.c |


For more information about how to configure these VxBus drivers beyond their defaults, refer to the [Wind River VxSim BSP Supplement Guide](https://docs.windriver.com/bundle/wr_vxsim_bsp_supplement_@vx_release "Wind River VxSim BSP Supplement Guide") and the VxSim User's Guide on the **Wind River Support Network**.


## Software Prerequisites

Before you begin, you must do the following:  

  * Install Wind River VxWorks on the workstation.  

  * Open a workstation command terminal and configure the environment variables for VxWorks command-line interface (CLI) development. Refer to the [Wind River VxSim BSP Supplement Guide](https://docs.windriver.com/bundle/wr_vxsim_bsp_supplement_@vx_release "Wind River VxSim BSP Supplement Guide") for details.    

  * Be familiar with the process for building and configuring VxWorks projects. For more information, see [VxWorks Configuration and Build Guide](https://docs.windriver.com/bundle/vxworks_configuration_and_build_guide_@vx_release "VxWorks Configuration and Build Guide").  


## Target Boot Procedure
This procedure is the most direct strategy for booting the target board successfully to the VxWorks kernel shell prompt. It can be done from Windows workstations.


### Step 2: Create the VxWorks Source Build Project
2.1 From the workstation command terminal, enter the following:

    C:\WindRiver\workspace> vxprj vsb create -bsp vxsim_windows myVSB -S
    C:\WindRiver\workspace> cd myVSB
    C:\WindRiver\workspace\myVSB> vxprj build
    C:\WindRiver\workspace\myVSB> cd ..


### Step 3: Create the VxWorks Image Project
3.1 From the workstation command terminal, enter the following:

    C:\WindRiver\workspace> vxprj create -vsb myVSB -profile PROFILE_DEVELOPMENT vxsim_windows myVIP
    C:\WindRiver\workspace> cd myVIP


### Step 4: Build the VxWorks Image Project
4.1 Build the U-Boot compatible VIP VxWorks image:

    C:\WindRiver\workspace\myVIP> vxprj build vxWorks


### Step 5: Launch VxSim
5.1 Change to the VxSim project default directory:

    C:\WindRiver\workspace\myVIP> cd default

5.2 Run vxSim:

    C:\WindRiver\workspace\myVIP\default> vxsim

5.3 Once VxWorks has booted, the banner becomes visible on the target console window.

    Target Name: vxTarget


                              VxWorks SMP 64-bit
            
                Copyright 1984-2021 Wind River Systems,  Inc.
            
                         Release version: 21.03
                              Build date: Mar  2 2021 10:16:04
                                   Board: 
                               CPU Count: 4
                          OS Memory Size: 
                        ED&R Policy Mode: Permanantly Deployed
                    
                    
                    
    Adding 9870 symbols for standalone.

    ->


**NOTE 1:** For a description of alternative BSP boot workflows, refer to the [Wind River VxSim BSP Supplement Guide](https://docs.windriver.com/bundle/wr_vxsim_bsp_supplement_@vx_release "Wind River VxSim BSP Supplement Guide") on the **Wind River Support Network**.

## Additional Documents
| Document Name                                                |
| :----------------------------------------------------------- |
| [Wind River VxSim BSP Supplement Guide](https://docs.windriver.com/bundle/wr_vxsim_bsp_supplement_@vx_release "Wind River VxSim BSP Supplement Guide") |
| [VxWorks Configuration and Build Guide](https://docs.windriver.com/bundle/vxworks_configuration_and_build_guide_@vx_release "VxWorks Configuration and Build Guide") |
| [Wind River VxSim User's Guide](https://docs.windriver.com/bundle/vxworks_vxsim_users_guide_@vx_release "Wind River VxSim User's Guide") |

