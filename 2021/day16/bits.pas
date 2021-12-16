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

Type
	TPacketData = record
		Length: SizeInt;
		Value, VersionSum: NativeInt
	end;

Function CalcPacketValue(PacketType: NativeInt; Current, Possible: NativeInt): NativeInt;
Begin
	Case PacketType of
		0: Result := Current + Possible;
		1: Result := Current * Possible;
		2: begin
			If Current < Possible then
				Result := Current
			else
				Result := Possible
		end;
		3: begin
			If Current > Possible then
				Result := Current
			else
				Result := Possible
		end;
		5: begin
			If Current > Possible then
				Result := 1
			else
				Result := 0
		end;
		6: begin
			If Current < Possible then
				Result := 1
			else
				Result := 0
		end;
		7: begin
			If Current = Possible then
				Result := 1
			else
				Result := 0
		end;
		Otherwise begin
			Writeln(stderr, 'Unrecognized packet type ', PacketType);
			Halt(1)
		end
	end		
End;

Function ProcessPacket(Bits: AnsiString; Pos: NativeInt): TPacketData;
Var
	PacketLen: SizeInt;
	PacketVer, PacketType, PacketValue: NativeInt;
	BitGroup, VersionSum: NativeInt;
	SubpacketCount, SubpacketLen: NativeInt;
	SubpacketData: TPacketData;
Begin
	PacketVer := BitsToDecimal(Bits, Pos, 3);
	PacketType := BitsToDecimal(Bits, Pos + 3, 3);
	
	PacketLen := 6;
	PacketValue := 0;
	VersionSum := PacketVer;

	If PacketType = 4 then begin
		Repeat
			BitGroup := BitsToDecimal(Bits, Pos + PacketLen, 5);
			PacketValue := (PacketValue * 16) + (BitGroup mod 16);
			PacketLen += 5
		Until BitGroup < 16
	end else begin
		If Bits[Pos + PacketLen] = '0' then begin
			SubpacketLen := BitsToDecimal(Bits, Pos + PacketLen + 1, 15);
			PacketLen += 16;

			SubpacketData := ProcessPacket(Bits, Pos + PacketLen);
			PacketLen += SubpacketData.Length;
			SubpacketLen -= SubpacketData.Length;

			VersionSum += SubpacketData.VersionSum;
			PacketValue := SubpacketData.Value;

			While SubpacketLen > 0 do begin
				SubpacketData := ProcessPacket(Bits, Pos + PacketLen);
				PacketLen += SubpacketData.Length;
				SubpacketLen -= SubpacketData.Length;

				VersionSum += SubpacketData.VersionSum;
				PacketValue := CalcPacketValue(PacketType, PacketValue, SubpacketData.Value)
			end
		end else begin
			SubpacketLen := BitsToDecimal(Bits, Pos + PacketLen + 1, 11);
			PacketLen += 12;

			SubpacketData := ProcessPacket(Bits, Pos + PacketLen);
			PacketLen += SubpacketData.Length;
			VersionSum += SubpacketData.VersionSum;
			PacketValue := SubpacketData.Value;
			
			For SubpacketCount := 2 to SubpacketLen do begin	
				SubpacketData := ProcessPacket(Bits, Pos + PacketLen);
				PacketLen += SubpacketData.Length;
				VersionSum += SubpacketData.VersionSum;

				PacketValue := CalcPacketValue(PacketType, PacketValue, SubpacketData.Value)
			end
		end;
	end;

	Result.Length := PacketLen;
	Result.Value := PacketValue;
	Result.VersionSum := VersionSum
End;

Procedure ProcessLine(Hex: AnsiString);
Var
	Bits: AnsiString;
	Packet: TPacketData;
Begin
	Bits := HexToBin(Hex);
	Packet := ProcessPacket(Bits, 1);

	Writeln(Hex);
	Writeln('Part1: ', Packet.VersionSum);
	Writeln('Part2: ', Packet.Value);
	Writeln()
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
