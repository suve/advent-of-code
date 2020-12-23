Program cups; {$MODE OBJFPC} {$COPERATORS ON}

Var
	Current: Integer;
	Next: Array[1..9] of Integer;
	MoveNo: Integer;

Function CharToInt(c: Char): Integer;
Begin
	Result := Ord(c) - 48
End;

Procedure ReadInput();
Var
	i: Integer;
	line: String;
Begin
	Readln(line);
	Current := CharToInt(line[1]);
	
	For i := 1 to 8 do begin
		Next[CharToInt(line[i])] := CharToInt(line[i+1])
	end;
	Next[CharToInt(line[9])] := CharToInt(line[1]);
End;

Procedure PerformMove();
Var
	firstPickedUp, destination: Integer;
Begin
	firstPickedUp := Next[Current];
	
	destination := Current;
	While(True) do begin
		destination -= 1;
		If(destination < 1) then begin
			{ 
				We want to end up with 9, but we do -1 at loop start,
				so we have to assign 10 here.
			}
			destination := 10; 
			continue
		end;		

		If(
			(destination <> firstPickedUp) and
			(destination <> Next[firstPickedUp]) and
			(destination <> Next[Next[firstPickedUp]])
		) then Break
	end;

	Next[Current] := Next[Next[Next[firstPickedUp]]];
	Next[Next[Next[firstPickedUp]]] := Next[Destination];
	Next[Destination] := firstPickedUp
End;

Procedure PrintState();
Var
	i, now: Integer;
Begin
	Write('Cups: ');
	
	now := Current;
	For i := 1 to 9 do begin
		Write(now, ' ');
		now := Next[now]
	end;

	Writeln('')
End;

Procedure PrintAnswer();
Var
	i, now: Integer;
Begin
	now := Next[1];
	For i := 1 to 8 do begin
		Write(now);
		now := Next[now]
	end;

	Writeln('')

End;

Begin
	ReadInput();
	For MoveNo := 1 to 100 do begin
		PrintState();
		PerformMove();
		Current := Next[Current]
	end;
	PrintState();
	PrintAnswer();
End.
