unit CBU_CommandLine;

{$Mode ObjFpc}

interface

type
	Command = Object
			Data: array of ANSIString;
			DataLength: Int64;
			
			procedure ParseCommandFromString(Str: ANSIString);
			function GetAsString(): ANSIString;
			procedure Empty();
			function MinusFirstParameter(): Command;
			function GetAsHumanString(): ANSIString;
		end;
	pCommand = ^Command; //We need this for axis_script. please dont touch...
	CommandList = Object
			CommandHistory: array of Command;
			CommandHistoryLength: Int64;
			
			procedure AddComand(Cmd: Command);
			function GetLastCommand(): Command;
		end;
	CommandLine = Object
			CommandHistory: CommandList;
			PromptPrefix: ANSIString;
			
			procedure InitializePrompt(Nme: ANSIString);
			procedure Prompt();
			function NextUserCommand(): Command;
			function NextCommandFromString(Dt: ANSIString): Command;
		end;
	pCommandLine = ^CommandLine;

function ShellProcessCommand(Cmd: Command): Command; //stub
function ParseCommandFromString(Dt: ANSIString): Command;
function CommandAsString(Cmd: Command): ANSIString;
function PadCommand(Cmd: Command; Width: Int64): Command;
function GetCommand_TrimLeft(Cmd: Command; Trim: Int64): Command;

implementation

uses
	crt,
	PM_Utils,
	PM_TextUtils;

function GetCommand_TrimLeft(Cmd: Command; Trim: Int64): Command;
var
	c: Int64;

begin
	GetCommand_TrimLeft := PadCommand(ParseCommandFromString(''), 1);
	if Cmd.DataLength <= Trim then
		Exit;
	
	c := 0;
	repeat
		c += 1;
		Cmd.Data[c] := Cmd.Data[c + Trim];
		until c + Trim >= Cmd.DataLength;
	Cmd.DataLength -= Trim;
	SetLength(Cmd.Data, Cmd.DataLength + 1);
	GetCommand_TrimLeft := Cmd;
end;

function Command.MinusFirstParameter(): Command;
var
	Cmd: Command;
	c: Int64;

begin
	Cmd.DataLength := DataLength;
	SetLength(Cmd.Data, DataLength + 1);
	c := 0;
	repeat
		c += 1;
		Cmd.Data[c] := Data[c+1];
		until c+1 >= DataLength;
	MinusFirstParameter := Cmd;
	Cmd.Empty();
end;

function PadCommand(Cmd: Command; Width: Int64): Command;
var
	c: Int64;
	
begin
	PadCommand := Cmd;
	SetLength(PadCommand.Data, Width + 1);
	c := 0;
	repeat
		c += 1;
		if Length(PadCommand.Data[c]) <= 0 then
			PadCommand.Data[c] := ' ';
		until c >= Width;
end;

function Command.GetAsString(): ANSIString;
begin
	GetAsString := CommandAsString(Self);
end;

function Command.GetAsHumanString(): ANSIString;
var
	c: Int64;
	
begin
	GetAsHumanString := '';
	if DataLength <= 0 then
		Exit;
	c := 0;
	repeat
		c += 1;
		GetAsHumanString += Data[c] + ' ';
		until c >= DataLength;
end;

procedure Command.Empty();
begin
	DataLength := 0;
	SetLength(Data, 0);
end;

function CommandAsString(Cmd: Command): ANSIString;
var
	c: Int64;
	
begin
	CommandAsString := '';
	if Cmd.DataLength <= 0 then
		Exit;
	c := 0;
	repeat
		c += 1;
		CommandAsString += '<' + Cmd.Data[c] + '> ';
		until c >= Cmd.DataLength;
end;

function ParseCommandFromString(Dt: ANSIString): Command;
var
	TempCommand: Command;
	
begin
	TempCommand.ParseCommandFromString(Dt);
	ParseCommandFromString := TempCommand;
end;

function CommandLine.NextCommandFromString(Dt: ANSIString): Command;
var
	TempCommand: Command;
	
begin
	TempCommand.ParseCommandFromString(Dt);
	TempCommand := ShellProcessCommand(TempCommand);
	CommandHistory.AddComand(TempCommand);
	NextCommandFromString := TempCommand;
end;

procedure CommandLine.InitializePrompt(Nme: ANSIString);
begin
	PromptPrefix := Nme;
end;

function ShellProcessCommand(Cmd: Command): Command; //stub
begin
	//We can preprocess for reinvocation here and other nifty stuff...
	ShellProcessCommand := Cmd;
end;

procedure CommandLine.Prompt();
begin
	if WhereX > 1 then
		writeln();
	write(PromptPrefix,'>');
end;

function CommandLine.NextUserCommand(): Command;
var
	LineIn: ANSIString;
	TempCommand: Command;
	
begin
	readln(LineIn);
	TempCommand.ParseCommandFromString(LineIn);
	TempCommand := ShellProcessCommand(TempCommand);
	CommandHistory.AddComand(TempCommand);
	NextUserCommand := TempCommand;
end;

function CommandList.GetLastCommand(): Command;
begin
	GetLastCommand := CommandHistory[CommandHistoryLength];
end;

procedure CommandList.AddComand(Cmd: Command);
begin
	CommandHistoryLength += 1;
	SetLength(CommandHistory, CommandHistoryLength + 1);
	CommandHistory[CommandHistoryLength] := Cmd;
end;

procedure Command.ParseCommandFromString(Str: ANSIString);
var
	Pos: Int64;
	CurrentWord: Int64;
	InsideQuotes: Boolean;

begin
	if Length(Str) <= 0 then
		begin
			Str := '  ';
		end;
	Pos := 0;
	DataLength := 0;
	repeat
		Pos := Pos + 1;
		if Str[Pos] = ' ' then
			DataLength := DataLength + 1;
		until Pos >= Length(Str);
	
	SetLength(Data, Pos + 1); //get 1 word headroom
	
	Pos := 0;
	InsideQuotes := False;
	CurrentWord := 1;
	repeat
		Pos := Pos + 1;
		if Str[Pos] = '''' then
			InsideQuotes := Not InsideQuotes;
		if (Str[Pos] <> ' ') and (ord(Str[Pos]) <> 13 or 8) and (Str[Pos] <> '	') and (Str[Pos] <> '''') then
				Data[CurrentWord] := Data[CurrentWord] + Str[Pos];
		if (Str[Pos] = ' ') and (InsideQuotes = True) and  (ord(Str[Pos]) <> 13 or 8) then
				Data[CurrentWord] += Str[Pos];
		if ((Str[Pos] = ' ') or (Ord(Str[Pos]) = 8)) and (InsideQuotes = False) then
			begin
				if CurrentWord > 0 then
					begin
						if length(Data[CurrentWord]) >= 1 then
							begin
								CurrentWord := CurrentWord + 1;
								Data[CurrentWord] := '';
							end;
					end;
			end;
		until Pos >= Length(Str);
	DataLength := CurrentWord;
end;

begin
end.
