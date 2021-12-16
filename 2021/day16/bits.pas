Program bits;

{$COPERATORS ON} {$MODE OBJFPC}

Uses
	SysUtils;

Function HexToBin(Hex: AnsiString): AnsiString;
Const
	BinaryDigit: Array[0..1] of Char = ('0', '1');
Var
	Idx, Len, Offset: SizeInt;
	Digit: NativeInt;
Begin
	Len := Length(Hex);
	SetLength(Result, Len * 4);

	For Idx := 1 to Len do begin
		Digit := Ord(Hex[Idx]);
		If (Digit >= Ord('0')) and (Digit <= Ord('9')) then
			Digit -= Ord('0')
		else if (Digit >= Ord('A')) and (Digit <= Ord('F')) then
			Digit := Digit - Ord('A') + 10
		else if (Digit >= Ord('a')) and (Digit <= Ord('f')) then
			Digit := Digit - Ord('a') + 10
		else begin
			Writeln(stderr, 'Invalid input: invalid hex digit "', Hex[Idx], '"');
			Halt(1);
		end;

		For Offset := 0 to 3 do begin
			Result[Idx*4 - Offset] := BinaryDigit[Digit mod 2];
			Digit := Digit div 2
		end
	end
End;

Function BitsToDecimal(BitsStr: AnsiString; Start, Count: NativeInt): NativeInt;
Begin
	Result := 0;
	While Count > 0 do begin
		Result := (Result * 2) + (Ord(BitsStr[Start]) - Ord('0'));
		Start += 1;
		Count -= 1
	end
End;

Procedure ProcessLine(Hex: AnsiString);
Var
	Bits: AnsiString;
	BitCount, Pos, PacketLen: SizeInt;
	PacketVer, PacketType: NativeInt;
	BitGroup, VersionSum: NativeInt;
Begin
	VersionSum := 0;

	Bits := HexToBin(Hex);
	BitCount := Length(Bits);

	Pos := 1;
	While Pos <= (BitCount - 6) do begin
		PacketVer := BitsToDecimal(Bits, Pos, 3);
		PacketType := BitsToDecimal(Bits, Pos + 3, 3);
		PacketLen := 6;

		If PacketType = 4 then begin
			Repeat
				BitGroup := BitsToDecimal(Bits, Pos + PacketLen, 5);
				PacketLen += 5
			Until BitGroup < 16
		end else begin
			If Bits[Pos + PacketLen] = '0' then
				PacketLen += 15
			else
				PacketLen += 11;
			PacketLen += 1;
		end;

		VersionSum += PacketVer;
		Pos += PacketLen
	end;

	Writeln(Hex, ' -> ', VersionSum)
End;

Var
	Line: AnsiString;

Begin
	While Not eof() do begin
		Readln(Line);
		Line := Trim(Line);
		if Line <> '' then ProcessLine(Line)
	end
End.
