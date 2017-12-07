Program tower;

Uses
	SysUtils, StrUtils;

Type
	PProgram = ^TProgram;
	TProgram = record
		Name: AnsiString;
		Weight: NativeInt;
		Parent: NativeInt;
		Children: Array of NativeInt;
	end;

Var
	Lines: Array of AnsiString;
	Prog: Array of TProgram;

Function SetParent(Const Name: AnsiString; Const ParentIdx: NativeInt):NativeInt;
Var
	Idx: NativeInt;
Begin
Writeln('-- setting parent for "', Name, '"');
	For Idx:=Low(Prog) to High(Prog) do begin
		If(Prog[Idx].Name = Name) then begin
			Prog[Idx].Parent := ParentIdx;
			Exit(Idx)
		end
	end;
	
	Exit(-1)
End;

Procedure ParseLine(Const Idx: NativeInt);
Var
	Line, ChildName: AnsiString;
	P, ChildrenNo, ChildIdx: NativeInt;
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
	
	ChildrenNo := 0;
	Repeat
		Delete(Line, 1, P);
		
		P := Pos(',', Line);
		If(P > 0) then
			ChildName := Trim(Copy(Line, 1, P-1))
		else
			ChildName := Trim(Line);
		
		ChildIdx := SetParent(ChildName, Idx);
		
		SetLength(Prog[Idx].Children, ChildrenNo+1);
		Prog[Idx].Children[ChildrenNo] := ChildIdx;
		ChildrenNo += 1
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
		Prog[Idx].Parent := -1;
		SetLength(Prog[Idx].Children, 0)
	end;
	
	For Idx:=0 to Len-1 do ParseLine(Idx);
End;

Function CalculateWeight(Const Idx: NativeInt):NativeInt;
Var
	Ch, chi, Count, Total: NativeInt;
	Weights: Array of NativeInt;
	AllEqual: Boolean;
Begin
	Count := Length(Prog[Idx].Children);
	If(Count = 0) then Exit(Prog[Idx].Weight);
	
	Total := Prog[Idx].Weight;
	
	SetLength(Weights, Count);
	For Ch := 0 to Count-1 do begin
		Weights[Ch] := CalculateWeight(Prog[Idx].Children[Ch]);
		Total += Weights[Ch]
	end;
	
	AllEqual := True;
	For Ch := 0 to Count-2 do begin
		For chi := Ch+1 to Count-1 do begin
			If(Weights[ch] <> Weights[chi]) then begin
				AllEqual := False;
				Break; break
			end
		end
	end;
	If(Not AllEqual) then begin
		Writeln('-- weights not equal');
		For Ch := 0 to Count-1 do begin
			Writeln('  -- ', Prog[Prog[Idx].Children[Ch]].Name, ': ', Weights[Ch], ' (', Prog[Prog[Idx].Children[Ch]].Weight, ')')
		end;
	end;
	
	Exit(Total)
End;

Procedure FindRoot();
Var
	Idx: NativeInt;
Begin
	For Idx:=Low(Prog) to High(Prog) do begin
		If(Prog[Idx].Parent < 0) then begin
			Writeln(Prog[Idx].Name, '; children: ', Length(Prog[Idx].Children));
			CalculateWeight(Idx);
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
