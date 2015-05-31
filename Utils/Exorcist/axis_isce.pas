unit axis_isce; //AXIS Integrate Serial Communication Extension (ISCE)

//This of ISCE has been modified to replace CBU_LASS without an AXIS environment

{$Mode ObjFpc}

interface

uses
	{$IfDef Linux}
		cthreads,
	{$EndIf}
	
	synaser;

type
	isce_container = object
			DevicePath: ANSIString;
			ForceMode: Boolean;
			Baud, Bits: Int64;
			Initialized, Connected, Active: Boolean;
			SerialInterface: TBlockSerial;
			RxTimeout: Int64;
			Redirect_Drakken: Boolean;
			
			procedure Initialize();
			procedure Connect();
			procedure Disconnect();
			procedure Empty();
			function CheckConfig(): Boolean;
			procedure SendString(Dt: ANSIString);
			function ReadString(): ANSIString;
			procedure SendBlock(Dt: ANSIString);
			function ReadBlock(): ANSIString;
			procedure UseDrakkenSystem();
		end;
	pisce_container = ^isce_container;
	ArduinoConnection = object //API Translation for drop in replacement of CBU_LASS
			ISCEContainer: isce_container;
			
			BaudRate, BitLength, LineTimeOut: Integer;
			Device: ANSIString;
			
			procedure LoadFromConfig(Src: ANSIString);
			procedure Connect(dev: ANSIString);
			procedure WriteToConfig(Src: ANSIString);
			procedure Disconnect();
			function ReadLine(): ANSIString;
			procedure SendLine(Dat: ANSIString);
			procedure PushConfig();
			procedure Empty();
			procedure PrintStatus();
			procedure Initialize();
		end;
	pArduinoConnection = ^ArduinoConnection;

implementation

uses
	PM_Threads;
	
procedure ArduinoConnection.Initialize();
begin
	ISCEContainer.Initialize();
end;

procedure isce_container.UseDrakkenSystem();
begin
	Redirect_Drakken := True;
	Initialized := True;
	Connected := True;
	Active := True;
	
	//CreateThread(@AXISData.SerialWorker, 'AXIS', 'SerialWorker');
	//AXISData.SerialWorker.SetTargetProcedure(@AXISData.PROC_SerialWorker);
	//AXISData.SerialWorker.Run();
end;

procedure ArduinoConnection.Disconnect();
begin
	ISCEContainer.Disconnect();
end;

procedure ArduinoConnection.PrintStatus();
begin
	writeln('ISCE: ArduCon.PrintStatus() stub!');
end;

procedure ArduinoConnection.Empty();
begin
	ISCEContainer.Empty();
end;

procedure ArduinoConnection.PushConfig();
begin
	ISCEContainer.Baud := BaudRate;
	
	//if ISCEContainer.CheckConfig() = false then
	//	writeln('ISCE: ArduinoConnection.Connect() passed bad config!')
	//else
		ISCEContainer.Connect();
end;

function ArduinoConnection.ReadLine(): ANSIString;
begin
	ReadLine := ISCEContainer.ReadString();
end;

procedure ArduinoConnection.SendLine(Dat: ANSIString);
begin
	ISCEContainer.SendString(Dat);
end;

procedure ArduinoConnection.WriteToConfig(Src: ANSIString);
begin
	writeln('ISCE: ArduCon.WriteToConfig() stub!');
end;

procedure ArduinoConnection.Connect(dev: ANSIString);
begin
	ISCEContainer.Initialize();
	ISCEContainer.DevicePath := dev;
	ISCEContainer.Bits := 8;
	ISCEContainer.RxTimeout := 1000;
	ISCEContainer.Baud := BaudRate;
	//if ISCEContainer.CheckConfig() = false then
	//	writeln('ISCE: ArduinoConnection.Connect() passed bad config!')
	//else
		ISCEContainer.Connect();
end;

procedure ArduinoConnection.LoadFromConfig(Src: ANSIString);
begin
	writeln('ISCE: ArduCon.LoadFromConfig() stub!');
end;

function isce_container.ReadBlock(): ANSIString;
begin
	Initialize();

	ReadBlock := SerialInterface.RecvBlock(RxTimeout);
end;

procedure isce_container.SendBlock(Dt: ANSIString);
begin
	Initialize();
	if Connected = True then
		SerialInterface.SendBlock(Dt);
end;

function isce_container.ReadString(): ANSIString;
begin
	Initialize();
	
	ReadString := '';
	if Connected = False then
		Exit;
	{
	ReadString := '';
	repeat
		ReadString += char(SerialInterface.RecvByte(RxTimeout));
		until SerialInterface.CanReadEx(RxTimeout);
	}
	
	//if Redirect_Drakken = False then
		ReadString := SerialInterface.RecvString(RxTimeout)
	//else
	//	ReadString := AXISData.KMod_Drakken^.ISCEInterface_GetStr();
end;

procedure isce_container.SendString(Dt: ANSIString);
var
	c: Int64;
	
begin
	Initialize();
	if Redirect_Drakken = False then
		begin
			c := 0;
			repeat
				c += 1;
				SerialInterface.SendByte(Ord(Dt[c]));
				until c >= Length(Dt);
		end;
end;

function isce_container.CheckConfig(): Boolean;
begin
	if Initialized = False then	
		begin
			CheckConfig := False;
			Exit;
		end
	else
		CheckConfig := True;
end;

procedure isce_container.Disconnect();
begin
	Initialize();
	Active := False;
	Connected := False;
	SerialInterface.Purge();
	SerialInterface.CloseSocket();
end;

procedure isce_container.Empty();
begin
	Initialize(); //avoid freeing memory that isnt allocated...
	Active := False;
	
	DevicePath := '';
	ForceMode := False;
	Baud := 0;
	Bits := 0;
	RxTimeout := 0;
	
	Connected := False;
	Initialized := False;
	SerialInterface.Free();
end;

procedure isce_container.Initialize();
begin
	if Initialized = True then
		Exit;
	
	Connected := False;
	Active := False;
	
	SerialInterface := TBlockSerial.Create();
	
	Initialized := True;
end;

procedure isce_container.Connect();
begin
	Initialize();
	if Connected = True then
		Exit;
	
	Connected := True;
	SerialInterface.Connect(DevicePath);
	SerialInterface.Config(Baud, Bits, 'N', SB1, false, false);
end;

begin
end.
