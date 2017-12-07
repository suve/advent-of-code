Program tower;

Uses
	SysUtils, StrUtils;

Type
	PProgram = ^TProgram;
	TProgram = record
		Name: AnsiString;
		Weight: NativeInt;
		Children: NativeInt;
		Parent: NativeInt
	end;

Var
	Lines: Array of AnsiString;
	Prog: Array of TProgram;

Procedure SetParent(Const Name: AnsiString; Const ParentIdx: NativeInt);
Var
	Idx: NativeInt;
Begin
Writeln('-- setting parent for "', Name, '"');
	For Idx:=Low(Prog) to High(Prog) do begin
		If(Prog[Idx].Name = Name) then begin
			Prog[Idx].Parent := ParentIdx;
			Exit()
		end
	end
End;

Procedure ParseLine(Const Idx: NativeInt);
Var
	Line, Child: AnsiString;
	P: NativeInt;
Begin
	Line := Lines[Idx];
	
	P := Pos('(', Line);
	// Prog[Idx].Name := Trim(Copy(Line, 1, P-1));
	Delete(Line, 1, P);
	
	P := Pos(')', Line);
	Prog[Idx].Weight := StrToInt(Copy(Line, 1, P-1));
	Delete(Line, 1, P);
	
	P := Pos('>', Line);
	If(P = 0) then Exit;
	
	Repeat
		Delete(Line, 1, P);
		
		P := Pos(',', Line);
		If(P > 0) then
			Child := Trim(Copy(Line, 1, P-1))
		else
			Child := Trim(Line);
		
		SetParent(Child, Idx);
		Prog[Idx].Children += 1
	Until (P=0);
End;

Procedure ReadInput();
Var
	Len: NativeInt;
	Line: AnsiString;
Begin
	Len := 0;
	While Not Eof do begin
		Readln(Line);
		
		SetLength(Lines, Len+1);
		Lines[Len] := Line;
		Len += 1
	end;
End;

Procedure ParseInput();
Var
	Idx, Parenthesis, Len: NativeInt;
Begin
	Len := Length(Lines);
	SetLength(Prog, Len);
	
	For Idx:=0 to Len-1 do begin
		Parenthesis := Pos('(', Lines[Idx]);
		Prog[Idx].Name := Trim(Copy(Lines[Idx], 1, Parenthesis-1));
		
		Prog[Idx].Weight := 0;
		Prog[Idx].Children := 0;
		Prog[Idx].Parent := -1
	end;
	
	For Idx:=0 to Len-1 do ParseLine(Idx);
End;

Procedure FindRoot();
Var
	Idx: NativeInt;
Begin
	For Idx:=Low(Prog) to High(Prog) do begin
		If(Prog[Idx].Parent < 0) then begin
			Writeln(Prog[Idx].Name, '; children: ', Prog[Idx].Children)
			Exit()
		end;
	end;
	
	Writeln('-- ERR --')
End;

Begin
	ReadInput();
	ParseInput();
	FindRoot()
End.
