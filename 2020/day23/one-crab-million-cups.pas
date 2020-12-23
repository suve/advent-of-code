Program cups; {$MODE OBJFPC} {$COPERATORS ON}

Const
	MILLION = 1000 * 1000;

Var
	Current: Integer;
	Next: Array[1..MILLION] of Integer;
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
	Next[CharToInt(line[9])] := 10;
	For i := 10 to MILLION-1 do begin
		Next[i] := i+1
	end;
	Next[MILLION] := CharToInt(line[1])
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
				We want to end up with MILLION, but we do -1 at loop start,
				so we have to assign (MILLION + 1) here.
			}
			destination := MILLION + 1; 
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

Procedure PrintAnswer();
Var
	first, second: Integer;
Begin
	first := Next[1];
	second := Next[first];

	Writeln(first, ' * ', second, ' = ', first * second)
End;

Begin
	ReadInput();
	For MoveNo := 1 to (10 * MILLION) do begin
		PerformMove();
		Current := Next[Current]
	end;
	PrintAnswer();
End.
