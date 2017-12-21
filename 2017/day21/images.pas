Unit Images;

{$MODE OBJFPC}

Interface

Type
	sInt = NativeInt;
	uInt = NativeUInt;
	
	PImage = ^TImage;
	TImage = object
		Private
			_data: PChar;
			_size: sInt;
			
			Function CountLitPixels():sInt;
			Function ToString(Const RowSeparator: Char):AnsiString;
			
		Public
			Function  GetSubImage(Const X, Y, SubSize: sInt):PImage;
			Procedure SetSubImage(Const X, Y: sInt; Const Source: PImage);
			
			Function Equals(Const Other: PImage):Boolean;
			
			Procedure FlipHorizontally();
			Procedure FlipVertically();
			Procedure RotateClockwise();
			Procedure RotateCounterClockwise();
			
			Function ToPrettyString():AnsiString;
			Function ToPatternString():AnsiString;
			
			Property PixelsLit: sInt read CountLitPixels;
			Property Size: sInt read _size;
			
			Constructor Create(Const DesiredSize: sInt);
			Constructor CreateCopyOf(Const Other: PImage);
			Constructor CreateFromPattern(Pattern: AnsiString);
			
			Destructor Destroy();
	end;

Implementation
	Uses Math;

Procedure CopyMem(Src, Dst: PChar; Size: uInt);
Begin
	While(Size > 0) do begin
		Dst^ := Src^;
		Dst += 1;
		Src += 1;
		
		Size -= 1
	end;
End;

Function TImage.GetSubImage(Const X, Y, SubSize: sInt):PImage;
Var
	pX, pY: sInt;
	SrcPos, DstPos: sInt;
Begin
	New(Result, Create(SubSize));
	
	DstPos := 0;
	For pY := 0 to (SubSize-1) do begin
		SrcPos := X + ((Y + pY) * Self._size);
		For pX := 0 to (SubSize-1) do begin
			// writeln(pX+X,':',pY+Y,' (', SrcPos, ') -> ',pX,':',py,' (',DstPos,')');
			
			Result^._data[DstPos] := Self._data[SrcPos];
			SrcPos += 1;
			DstPos += 1
		end;
	end
End;

Procedure TImage.SetSubImage(Const X, Y: sInt; Const Source: PImage);
Var
	pX, pY: sInt;
	SrcPos, DstPos: sInt;
Begin
	SrcPos := 0;
	For pY := 0 to (Source^._size-1) do begin
		DstPos := X + ((Y + pY) * Self._size);
		For pX := 0 to (Source^._size-1) do begin
			Self._data[DstPos] := Source^._data[SrcPos];
			SrcPos += 1;
			DstPos += 1
		end;
	end
End;

Function TImage.Equals(Const Other: PImage):Boolean;
Begin
	If(Self._size <> Other^._size) then
		Result := False
	else
		Result := (CompareChar(Self._data^, Other^._data^, _size * _size) = 0)
End;

Procedure TImage.FlipHorizontally();
Var
	HalfSize, Y, X, ay, src, dst: sInt;
	Tmp: Char;
Begin
	HalfSize := (_size-1) div 2;
	For X:=0 to (_size-1) do begin
		For Y:=0 to HalfSize do begin
			Src := (Y * _size) + X;
			
			ay := _size - 1 - Y;
			Dst := (ay * _size) + X;
			
			Tmp := _data[Dst];
			_data[Dst] := _data[Src];
			_data[Src] := Tmp
		end
	end
End;

Procedure TImage.FlipVertically();
Var
	HalfSize, Y, X, ax, src, dst: sInt;
	Tmp: Char;
Begin
	HalfSize := (_size-1) div 2;
	For Y:=0 to (_size-1) do begin
		For X:=0 to HalfSize do begin
			Src := (Y * _size) + X;
			
			ax := _size - 1 - X;
			Dst := (Y * _size) + ax;
			
			Tmp := _data[Dst];
			_data[Dst] := _data[Src];
			_data[Src] := Tmp
		end
	end
End;

Procedure TImage.RotateClockwise();
Var
	cpy: PChar;
	srcX, srcY, dstX, dstY: sInt;
Begin
	GetMem(cpy, _size * _size);
	CopyMem(_data, cpy, _size * _size);
	
	For srcY:=0 to (_size-1) do begin
		dstX := _size - 1 - srcY;
		For srcX:=0 to (_size-1) do begin
			dstY := srcX;
			
			_data[(dstY * _size) + dstX] := cpy[(srcY * _size) + srcX]
		end
	end;
	
	FreeMem(cpy, _size * _size)
End;

Procedure TImage.RotateCounterClockwise();
Var
	cpy: PChar;
	srcX, srcY, dstX, dstY: sInt;
Begin
	GetMem(cpy, _size * _size);
	CopyMem(_data, cpy, _size * _size);
	
	For srcY:=0 to (_size-1) do begin
		dstX := _size - 1 - srcY;
		For srcX:=0 to (_size-1) do begin
			dstY := _size - 1 - srcX;
			
			_data[(dstY * _size) + dstX] := cpy[(srcY * _size) + srcX]
		end
	end;
	
	FreeMem(cpy, _size * _size)
End;

Function TImage.CountLitPixels():sInt;
Var
	Idx, Count: sInt;
Begin
	Count := 0;
	For Idx := 0 to (_size * _size)-1 do
		If(_data[Idx] = '#') then
			Count += 1;
	
	Result := Count
End;

Function TImage.ToString(Const RowSeparator: Char):AnsiString;
Var
	Pos: sInt;
Begin
	Result := '';
	For Pos:=0 to (_size * _size)-1 do begin
		If(Pos > 0) and ((Pos mod _size) = 0) then Result += RowSeparator;
		Result += _data[Pos]
	end
End;

Function TImage.ToPrettyString():AnsiString;
Begin
	Result := Self.ToString(#10) + #10
End;

Function TImage.ToPatternString():AnsiString;
Begin
	Result := Self.ToString('/')
End;

Constructor TImage.Create(Const DesiredSize: sInt);
Begin
	_size := DesiredSize;
	_data := GetMem(_size * _size);
	
	FillChar(_data^, _size * _size, '.')
End;

Constructor TImage.CreateCopyOf(Const Other: PImage);
Begin
	_size := Other^._size;
	_data := GetMem(_size * _size);
	
	CopyMem(Other^._data, Self._data, _size * _size)
End;

Constructor TImage.CreateFromPattern(Pattern: AnsiString);
Var
	Slash, pSize: sInt;
	patternChars: PChar;
Begin
	pSize := 1;
	While True do begin
		Slash := Pos('/', Pattern);
		If(Slash = 0) then Break;
		
		Delete(Pattern, Slash, 1);
		pSize += 1
	end;
	
	_size := pSize;
	_data := GetMem(_size * _size);
	
	patternChars := @Pattern[1];
	CopyMem(patternChars, Self._data, _size * _size)
End;

Destructor TImage.Destroy();
Begin
	FreeMem(_data, _size * _size)
End;

End.
