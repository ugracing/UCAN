program Exorcist;

{$Mode ObjFpc}

uses
	axis_isce,
	CBU_CommandLine,
	PM_Utils,
	crt;

type
	objProc = procedure of Object;
	Proc = procedure;
	DBGCodeList = Record
			TextID: array of ANSIString;
			DBGID: array of ANSIString;
			DLength: Int64;
		end;

var
	Link: ArduinoConnection;
	TgtPort: ANSIString;
	LineIn: ANSIString;
	CmdIn: Command;
	DBGCodeDB: DBGCodeList;
	StartTime: Int64;

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
	
	GoToXY(50, c);
	writeln('[done!]');
end;

procedure ObjInit(Msg: ANSIString; Tgt: objProc);
var
	c: Int64;
	
begin
	write(Msg);
	c := WhereY();
	
	Tgt();
	
	GoToXY(50, c);
	writeln('[done!]');
end;

procedure Init_DBGAlias();
begin
	LoadDBGCodes('ucandebug.h');
end;

begin
	writeln('UCAN Debug Helper - Exorcist');
	
	ObjInit('Initialize ISCE', @Link.Initialize);
	CorInit('Initialize debug aliases from ucandebug.h', @Init_DBGAlias);
	
	writeln('Exorcist INIT complete.');
	writeln('Hit <ESC> to exit.');
	writeln('');
	
	write('Connection port:');
	readln(TgtPort);
	writeln('Connecting to ',TgtPort,' with:');
	writeln('	bits    : 8');
	writeln('	baud    : 9600');
	writeln('	Timeout : 2000');
	with Link do
		begin
			BaudRate := 9600;
			LineTimeout := 2000;
			BitLength := 8;
		end;
	Link.Connect(TgtPort);
	writeln('Connected!');
	writeln('');
	StartTime := GetTime();

	repeat
		LineIn := Link.ReadLine();
		if Length(LineIn) > 0 then
			begin
				CmdIn := ParseCommandFromString(LineIn);
				CmdIn := PadCommand(CmdIn, 5);
				CmdIn.Data[2] := ResolveDBGCode(CmdIn.Data[2]);
				if CmdIn.Data[2] = 'UCAN_Debug_Boot' then
					begin
						writeln('');
						writeln('Detected UCAN device reboot!');
						writeln('');
						StartTime := GetTime();
					end;
				writeln(GetTime() - StartTime,': ',CmdIn.GetAsHumanString());
			end;
		
		if Keypressed() then
			begin
				if ReadKey() = #27 then
					Break;
			end;
		
		until 9-8=7;
	Link.Disconnect();
end.
