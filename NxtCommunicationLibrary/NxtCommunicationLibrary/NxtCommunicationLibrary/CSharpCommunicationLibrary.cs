using System;
using MonoBrick.NXT;

namespace NxtCommunicationLibrary
{
    public class CSharpCommunicationLibrary : ICSharpCommunicationLibrary
    {
        private readonly Brick<TouchSensor, NoSensor, NXTColorSensor, NXTSoundSensor> nxtBrick;

        public CSharpCommunicationLibrary(string comPort)
        {
            nxtBrick = new Brick<TouchSensor, NoSensor, NXTColorSensor, NXTSoundSensor>(comPort);
            nxtBrick.Connection.Open();
        }

        ~CSharpCommunicationLibrary()
        {
            if (nxtBrick.Connection.IsConnected)
            {
                nxtBrick.Connection.Close();
            }
        }

        public void StartMotor(string motor, sbyte speed, uint degrees)
        {
            var nxtMotor = GetNxtMotorFromString(motor);
            nxtMotor?.On(speed, degrees);
        }
        public void StopMotor(string motor)
        {
            var nxtMotor = GetNxtMotorFromString(motor);
            nxtMotor?.Brake();
        }

        private Motor GetNxtMotorFromString(string motor)
        {
            Motor nxtMotor;
            switch (motor.ToUpper())
            {
                case "A":
                    nxtMotor = nxtBrick.MotorA;
                    break;
                case "B":
                    nxtMotor = nxtBrick.MotorB;
                    break;
                case "C":
                    nxtMotor = nxtBrick.MotorC;
                    break;
                default:
                    nxtMotor = null;
                    break;
            }
            return nxtMotor;
        }
    }
}
