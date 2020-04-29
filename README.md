# Spirometer
The purpose of the ASPIRE-ometer is to assess lung health and capability through the measurement of several parameters, including lung volumes and flow rates during normal, full capacity, and forceful breaths.

The device works by measuring the pressure differential across a thin wire screen placed in the airflow path. This pressure differential is directly proportional to flow rate and can be manipulated to give volume. The ASPIRE-ometer is user-friendly, guiding the user through calibration and testing step-by-step and allowing repetition and correction for mistakes. After selecting component parameters to maximize resolution and minimize noise, the device was tested on two subjects with two separate pneumotachs. All measurements were found to be consistent across the pneumotachs, and all values were similar to those available in literature.

## Overview
The spirometer is a device used to measure respiratory parameters, especially in the context of assessing lung health. It can differentiate between inhalation and exhalation values and measure both flow rates and volumes. The tool is often used to test for lung diseases, including asthma, cystic fibrosis, and Chronic Obstructive Pulmonary Disease (COPD), or to perform clinical Pulmonary Function Tests (PFTs) to quantify lung fitness. The measurements can then be used to prescribe a ventilator or medication or to assess lung health during treatment.

In the ASPIRE-ometer, Tidal Volume, Inspiratory and Expiratory Capacity, and peak inhalation and exhalation flows are calculated. Tidal Volume (VT) measures the amount of air ventilated in normal quiet breath. Inspiratory and Expiratory Capacity (IC/EC) are the maximum amounts of air that can be inspired or expired, respectively.

## Measurements
To measure these parameters, the spirometer contains a transducer known as a pneumotachometer, which linearly
transduces airflow into differential pressure. To do this, pressure is sampled on both sides of a fine wire screen inserted into the airflow. This screen disrupts the flow to create the pressure differential. In this spirometry device, the transducer used was the MPX2010DP, which uses two input ports, one on each side of the screen, a strain gage, and a Wheatstone bridge to convert the differential pressure into a proportional differential voltage signal. After signal processing, a microprocessor can then read the user-friendly output of flowrates, which can be converted to volumes.

The system must show zero flow when airflow is indeed zero. As such, the DC offset must be removed from every measurement. In addition, the device must be calibrated with a known volume in order to report accurate volumes and flows. For maximum accuracy, two calibration constants are calculated due to potential differences in flowrate: K+ for inhalation and K- for exhalation.

## User Interface
The overall goal of the ASPIRE-ometer was to measure and display several typical spirometer measurements through a user-friendly interface. The user is carefully guided through each step of testing and prompted to error-check values along the way. If needed, restarting the device, retesting, or redisplaying results only requires the click of a button. The ASPIRE-ometer provides a quick and easy experience in a mobile package – perfect for any user in any place.

# Instructions for Use
From the beginning, the user can choose their testing path and proceed at their own pace – through calibration, pneuomotach selection, and options to restart, repeat testing, or display results again. To ensure accuracy and precision, no knowledge is presumed of the user. Values are constantly checked automatically or via user interaction, and the user can repeat steps if measurements appear incorrect.

## Calibration
### Manual
To begin, choose whether to calibrate manually (`R: yes`) or to use pre-calibrated calibration constants (`L: no`). To calibrate manually, follow the onscreen instructions using the 3 L calibration syringe. First connect the syringe to the pneumotach in the orientation denoted on the pneumotach (the side marked). To obtain K+, press the up button and then pull the syringe handle, fully extending the rod over three seconds. The pull should be complete when the screen displays “`stop pull`.” K+ will then be displayed. To obtain K-, follow a similar procedure. Press the down button when ready and push the syringe handle, fully compressing the rod over three seconds. The push should be complete when the screen displays “`stop push`.” K- will then be displayed. As the screen directs, if the magnitude of the displayed calibration constants is not between 10 and 20, the device should be recalibrated (`R: yes`). If the calibration constants are in range, the device is ready for testing (`L: test`).
<p align="center">
  <img width="400" height="300" src="/Images/CalibrationScreen.png">
</p>

### Automatic
If manual calibration is chosen, the pneumotach in use must be selected to ensure correct calibration constants. If these constants do not match the pneumotach in use, measurements will be inaccurate. Two pneumotachs are pre-programmed (`L: 1, R: 2`) The third selection contains the latest stored constants by the user (`Up: User`). Once this selection is made, the device is ready for testing (`L: test`).
<p align="center">
  <img width="400" height="300" src="/Images/PickPneumScreen.png">
</p>

### Offset Failure
If manual calibration is selected, offset calculation occurs before calibration. If automatic calibration is selected, offset calculation occurs after pneumotach selection. If offset calculation fails, the device will display “`Offset failure. Restarting`,” and the device will restart to the “`Calibrate?`” screen. The user can then proceed from the beginning. If this occurs more than twice, the device should be restarted.

## Testing
Before beginning testing, live flowrate is displayed onscreen in L/min. If not breathing, the flow rate should be close to zero. If it is not, straighten the pneumotach tubing. When ready, press the left button to begin testing. The first test is for tidal volume. Take one normal breath when directed. The user has five seconds. The second test is for inspiratory and expiratory capacity. Take one full capacity breath, filling up the lungs and expelling as much air as possible when directed. The user again has five seconds. The third and final test is for peak flow. Take one forceful breath when directed, inspiring and expelling air as fast as possible. The allotted time remains five seconds.

## Results
Results will be automatically displayed in order of testing: tidal volume, inspiratory capacity, expiratory capacity, peak flow while inhaling, and peak flow while exhaling. After scrolling is complete, a menu will appear asking if the user would like to restart the device (`Up: Reset`), repeat the results (`L: Results`), or repeat testing (`R: Test`). Restarting will return the spirometer to the initial “`Calibrate?`” screen. Results will display the results in order again. Test will repeat the three tests in sequence, followed by results.
