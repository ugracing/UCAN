program Exorcist;

{$Mode ObjFpc}

uses
	axis_isce,
	CBU_CommandLine,
	PM_Utils,
	PM_TextUtils,
	crt;

type
	objProc = procedure of Object;
	Proc = procedure;
	DBGCodeList = Record
			TextID: array of ANSIString;
			DBGID: array of ANSIString;
			DLength: Int64;
		end;
	ArANSIString = Array of ANSIString;
	pArANSIString = ^ArANSIString;

var
	Link: ArduinoConnection;
	TgtPort: ANSIString;
	LineIn: ANSIString;
	CmdIn: Command;
	DBGCodeDB: DBGCodeList;
	StartTime: Int64;
	k: Char;

function TabStrip(Data: ANSIString): ANSIString;
var
	c: Int64;

begin
	TabStrip := ' ';
	if Length(Data) <= 0 then
		Exit;
	c := 0;
	repeat
		c += 1;
		if Data[c] = '	' then
			TabStrip += ' '
		else
			TabStrip += Data[c];
			
		until c >= Length(Data);
end;

//reuses LineIn from global. Nasty but saves memory & programmer time
procedure LoadDBGCodes(Src: ANSIString);
var
	Disk: Text;
	Cmd: Command;
	
begin
	Assign(Disk, Src);
	Reset(Disk);
	repeat
		readln(Disk, LineIn);
		if ((Length(LineIn) > 0) and (LineIn[1] = '#')) then
			begin
				Cmd := ParseCommandFromString(TabStrip(LineIn));
				
				DBGCodeDB.DLength += 1;
				SetLength(DBGCodeDB.TextID, DBGCodeDB.DLength + 1);
				SetLength(DBGCodeDB.DBGID, DBGCodeDB.DLength + 1);
				DBGCodeDB.TextID[DBGCodeDB.DLength] := Cmd.Data[2];
				DBGCodeDB.DBGID[DBGCodeDB.DLength] := Cmd.Data[3];
			end;
		until eof(Disk);
	Close(Disk);
end;

function ResolveDBGCode(Code: ANSIString): ANSIString;
var
	c: Int64;

begin
	c := 0;
	repeat
		c += 1;
		
		if Code = DBGCodeDB.DBGID[c] then
			begin
				ResolveDBGCode := DBGCodeDB.TextID[c];
				Exit;
			end;
		
		until c >= DBGCodeDB.DLength;
	ResolveDBGCode := Code;
end;

procedure CorInit(Msg: ANSIString; Tgt: Proc);
var
	c: Int64;
	
begin
	write(Msg);
	c := WhereY();
	
	Tgt();
	
	GoToXY(70, c);
	writeln('[done!]');
end;

procedure ObjInit(Msg: ANSIString; Tgt: objProc);
var
	c: Int64;
	
begin
	write(Msg);
	c := WhereY();
	
	Tgt();
	
	GoToXY(70, c);
	writeln('[done!]');
end;

procedure Init_Unlock();
var
	Suffix: ANSIString;
	LFile: Text;
	
begin
	Suffix := TgtPort[Length(TgtPort) - 3..Length(TgtPort)];
	if DoesFileExist('/var/lock/LCK..tty' + Suffix) = True then
		begin
			writeln('Cleaning [','/var/lock/LCK..tty' + Suffix,']...');
			{$I-}
				Assign(LFile, '/var/lock/LCK..tty' + Suffix);
				Erase(LFile);
				Close(LFile);
			{$I+}
			writeln('Cleaned lock file [','/var/lock/LCK..tty' + Suffix,']');
		end;
end;

procedure Init_DBGAlias();
begin
	LoadDBGCodes('ucandebug.h');
end;

procedure Colourizer(Line: ANSIString);
begin
	if FindOccurencesInString(Upcase(Line), 'ERROR') > 0 then
		TextColor(Red);
end;


function YNPrompt(Item: ANSIString): Boolean;
var
	c, lc: Char;
	ConditionMet: Boolean = False;
	
begin
	if WhereX() > 1 then
		GoToXY(1, WhereY());
		
	TextBackGround(Red);
	ClrEol();
	
	c := 'n';
	
	write(Item);
	
	GoToXY(40, WhereY());
	
	write('[y/N]');
	TextBackGround(Black);
	ClrEol();
	
	GoToXY(50, WhereY());
	repeat
		lc := c;
		c := ReadKey();
		if (ord(c) <> 13) and (ord(c) <> 8) then
			write(c);
			
		if ((c = 'y') or (c = 'n'))then
			ConditionMet := True;
			
		until ConditionMet = True;
	
	GoToXY(1, WhereY());
		
	TextBackGround(Black);
	ClrEol();
	
	write(Item);
	
	GoToXY(40, WhereY());
	
	if c = 'y' then
		TextBackGround(Green)
	else
		TextBackGround(Red);
		
	write(lc);
	TextBackGround(Black);
	
	writeln('');
	
	if c = 'y' then
		YNPrompt := True
	else
		YNPrompt := False;
end;

procedure DrawStrList(pBuf: pArANSIString; Sz: Int64);
var
	c: Int64;

begin
	c := 0;
	repeat
		c += 1;
		writeln(' ',c,'	 - ',pBuf^[c]);
		until c >= Sz;
end;

function PortList(pBuf: pArANSIString): Int64;
var
	c, n: Int64;
	
begin	
	c := 0;
	n := 0;
	repeat
		if DoesFileExist('/dev/ttyUSB' + IntToStr(c)) then
			begin
				n += 1;
				pBuf^[n] := '/dev/ttyUSB' + IntToStr(c);
			end;
		c += 1;
		until c >= 10;
		
	c := 0;
	repeat
		if DoesFileExist('/dev/ttyACM' + IntToStr(c)) then
			begin
				n += 1;
				pBuf^[n] := '/dev/ttyACM' + IntToStr(c);
			end;
		c += 1;
		until c >= 10;
	
	PortList := n;
end;

procedure ConnectionMenu();
var
	c, Sel: Int64;
	ConPort: array of ANSIString;
	ConPorts: Int64;
	LineIn: ANSIString;
	
begin
	ClrScr();
	writeln('Connection port:');
	writeln('       (Hit [`] to refresh');
	writeln('');
	
	SetLength(ConPort, 21);
	ConPorts := PortList(@ConPort);
	
	DrawStrList(@ConPort, ConPorts);
	
	Sel := 1;
	repeat	
		c := 0;
		repeat
			GoToXY(1, c + 3);
			write(' ');
			c += 1;
			until c >= 21;
			
		GoToXY(1, Sel + 3);
		write('*');
		
		k := ReadKey();
		if ord(k) = 13 then
			TgtPort := ConPort[Sel];
		if k = '`' then
			begin
				ConPorts := PortList(@ConPort);
				
				ClrScr();
				writeln('Connection port:');
				writeln('       (Hit [`] to refresh');
				writeln('');
				
				DrawStrList(@ConPort, ConPorts);
			end;
			
		if (ord(k) = 72) and (Sel > 1) then
			Sel -= 1;
		if (ord(k) = 80) and (Sel < ConPorts) then
			Sel += 1;
			
		until ord(k) = 13;
	
	ClrScr();
end;

procedure ConfMenu();
var
	Baud, Timeout, Sel: Int64;
	SubSel: array [1..2] of Int64;
	
begin
	ClrScr();
	writeln('Connecting to ',TgtPort,' with:');
	writeln('     Bits    : 8');
	writeln('     Baud    : 9600');
	writeln('     Timeout : 2000');
	
	Baud := 9600;

	Sel := 4;
	
	Timeout := 2000;
	
	repeat
		ClrScr();
		writeln('Configure: ');
		writeln('   Bits      : 8');
		writeln('   Baud      : ', Baud);
		writeln('   Timeout   : ', TimeOut);
		writeln('   Accept');
		
		GoToXY(1, Sel + 1);
		if Sel < 4 then
			write('<>')
		else
			write('*');
		
		k := ReadKey();
		if (ord(k) = 72) and (Sel > 1) then
			Sel -= 1
		else if (ord(k) = 80) and (Sel < 4) then
			Sel += 1;
		
		if (ord(k) = 75) and (Sel = 2) and (Baud > 9600) then
			Baud -= 9600
		else if (ord(k) = 77) and (Sel = 2) and (Baud < 115200) then
			Baud += 9600;
			
		if (ord(k) = 75) and (Sel = 3) and (Timeout > 100) then
			Timeout -= 100
		else if (ord(k) = 77) and (Sel = 3) then
			Timeout += 100;
		
		until ord(k) = 13;
	
	Link.BaudRate := Baud;
	Link.LineTimeout := Timeout;
end;

var
	wy: Int64;

begin
	ClrScr();
	writeln('*-----------------------------------------------------------------------------*');
	writeln('                       UCAN Debug Helper - Exorcist 1.0');
	writeln('');
	
	ObjInit('Initialize ISCE', @Link.Initialize);
	CorInit('Initialize debug aliases from ucandebug.h', @Init_DBGAlias);
	
	writeln('Exorcist INIT complete.');
	writeln('*-----------------------------------------------------------------------------*');
	writeln('Hit <ESC> to exit.');
	writeln('');
	Pause(1000);
	
	ConnectionMenu();
	ConfMenu();
	
	writeln('Connection port:', TgtPort);
	
	CorInit('Clearing relevant lock files', @Init_Unlock);
	
	Pause(1000);
	
	Link.BitLength := 8;
	Link.Connect(TgtPort);
	writeln('Connected!');
	writeln('');
	StartTime := GetTime();

	repeat
		LineIn := Link.ReadLine();
		if Length(LineIn) > 0 then
			begin
				CmdIn := ParseCommandFromString(LineIn);
				CmdIn := PadCommand(CmdIn, 20);
				CmdIn.Data[2] := ResolveDBGCode(CmdIn.Data[2]);
				if CmdIn.Data[2] = 'UCAN_Debug_Boot' then
					begin
						writeln('');
						writeln('Detected UCAN boot message!');
						writeln('');
						StartTime := GetTime();
					end;
				Colourizer(CmdIn.GetAsHumanString());
				writeln(GetTime() - StartTime,': ',CmdIn.GetAsHumanString());
				TextColor(Lightgray);
				
				wy := WhereY();
				GoToXY(1, 1);
				TextBackGround(Blue);
				ClrEol();
				write(TgtPort + ': Hit <ESC> to exit, <SPACE> to pause');
				TextBackGround(Black);
				GoToXY(1, wy);
			end;
		
		if Keypressed() then
			begin
				k := ReadKey();
				if k = ' ' then
					begin
						while Keypressed() = false do
							Pause(1);
						k := ReadKey();
					end;
					
				if k = #27 then
					Break;
					
				k := #0;
			end;
		
		until 9-8=7;
	Link.Disconnect();
end.
