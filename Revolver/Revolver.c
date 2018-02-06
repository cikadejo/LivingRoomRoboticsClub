#pragma config(Sensor, port2,  colorSensor,    sensorVexIQ_ColorHue)
#pragma config(Motor,  motor1,          leftDrive,     tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor5,          rightDrive,    tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor6,          revolver,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,         dumper,        tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor11,         fifthWheel,    tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         conveyor,      tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void setDriveMotorsState()
{
		bool isHorizontalDrive = abs(vexRT[ChC]) > 50;

		if (isHorizontalDrive == true)
		{
				if (vexRT[ChC] > 0)
				{
						motor[fifthWheel] = vexRT[ChC] - 50;
				}
				else
				{
						motor[fifthWheel] = vexRT[ChC] + 50;
				}

				motor[leftDrive] = 0;
				motor[rightDrive] = 0;
		}
		else
		{
				motor[fifthWheel] = 0;

				motor[leftDrive] = vexRT[ChA];
				motor[rightDrive] = vexRT[ChD];
		}
}

void setConveyorBeltMotorState(bool* pIsRevolverInAutoMode)
{
		if (vexRT[BtnRUp] == true)
		{
				motor[conveyor] = 100;
				*pIsRevolverInAutoMode = true;
		}
		if (vexRT[BtnRDown] == true)
		{
				motor[conveyor] = 0;
				*pIsRevolverInAutoMode = false;
		}
}

void setRevolverMotorStateInManualMode(bool* pIsRevolverInAutoMode)
{
		int increment = 0;
		if (vexRT[BtnFUp] == true)
		{
				increment = 1;
		}
		if (vexRT[BtnFDown] == true)
		{
				increment = -1;
		}

		if (increment != 0)
		{
				*pIsRevolverInAutoMode = false;

				int stepCount = ((int)(getMotorEncoder(revolver) + 20)) / 60;
				stepCount += increment;

				setMotorTarget(revolver,  stepCount * 60, 100);
		}
		else
		{
				static int previousFineAdjustmentSpeed = 0;
				int fineAdjustmentSpeed = 0;
				if (vexRT[BtnEUp] == true)
				{
						fineAdjustmentSpeed = 15;
				}
				else if (vexRT[BtnEDown] == true)
				{
						fineAdjustmentSpeed = -15;
				}

				if (fineAdjustmentSpeed != 0)
				{
						*pIsRevolverInAutoMode = false;
						motor[revolver] = fineAdjustmentSpeed;
				}
				else if (previousFineAdjustmentSpeed != 0)
				{
						motor[revolver] = 0;
						resetMotorEncoder(revolver);
				}

				previousFineAdjustmentSpeed = fineAdjustmentSpeed;
		}
}


void setRevolverMotorStateInAutoMode()
{
		int blueChannel = getColorBlueChannel(port2);
		int redChannel = getColorRedChannel(port2);
		int greenChannel = getColorGreenChannel(port2);

		writeDebugStreamLine("%3d    %3d     %3d", redChannel, greenChannel, blueChannel);

		if (blueChannel > redChannel && blueChannel > greenChannel && blueChannel > 5)
		{
				// blue
				setMotorTarget(revolver, 120, 100);

		}
		else if (redChannel > blueChannel && redChannel > greenChannel && redChannel > 5)
		{
				// red
				setMotorTarget(revolver, -120, 100);
		}
		else if (greenChannel > 5)
		{
				// green
				setMotorTarget(revolver, 0, 100);
		}
}

void setDumperMotorState()
{
		if (vexRT[BtnLUp] == true)
		{
				motor[dumper] = 100;
		}
		else if (vexRT[BtnLDown] == true)
		{
				motor[dumper] = -100;
		}
		else
		{
				motor[dumper] = 0;
		}
}

task main()
{
		setColorMode(port2, colorTypeRGB_Hue_Reflected);
		bool isRevolverInAutoMode = false;

		while (true)
		{
			setDriveMotorsState();
			setConveyorBeltMotorState(&isRevolverInAutoMode);
			setRevolverMotorStateInManualMode(&isRevolverInAutoMode);
			if (isRevolverInAutoMode == true)
			{
					setRevolverMotorStateInAutoMode();
			}
			setDumperMotorState();
		}
}
