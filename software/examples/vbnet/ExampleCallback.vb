Imports System
Imports Tinkerforge

' FIXME: This example is incomplete

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LED Strip Bricklet 2.0

    ' Use frame started callback to move the active LED every frame
    Sub FrameStartedCB(ByVal sender As BrickletLEDStripV2, ByVal length As Integer)
        Console.WriteLine("Length: " + length.ToString())
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ls As New BrickletLEDStripV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set frame duration to 50ms (20 frames per second)
        ls.SetFrameDuration(50)

        ' Register frame started callback to subroutine FrameStartedCB
        AddHandler ls.FrameStartedCallback, AddressOf FrameStartedCB

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
