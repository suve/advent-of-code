Program buglife;

{$MODE OBJFPC} {$COPERATORS ON}

Type
	PTile = ^TTile;
	TTile = (
		TILE_EMPTY,
		TILE_BUG
	);

	PMap = ^TMap;
	TMap = Object
		Private
			_data: Array[0..24] of TTile;

			Function Offset(X, Y: NativeInt):NativeInt;

			Function GetTile(X, Y: NativeInt):TTile;
			Procedure SetTile(X, Y: NativeInt; T:TTile);
		
		Public
			Procedure Print();
			Function GetBiodiversity():NativeUInt;

			Function Equals(Const Other:TMap):Boolean;

			Property Tile[X, Y: NativeInt]: TTile
				read GetTile;

			Constructor CreateFromString(Const Str:String);
			Constructor CreateSimulateOther(Const Other: TMap);
			Destructor Destroy();
	end;

Function TMap.Offset(X, Y: NativeInt):NativeInt;
Begin
	Result := (Y * 5) + X
End;

Function TMap.GetTile(X, Y: NativeInt):TTile;
Begin
	If (X < 0) or (X >= 5) then 
		Result := TILE_EMPTY
	Else If (Y < 0) or (Y >= 5) then
		Result := TILE_EMPTY
	Else
		Result := _data[Offset(X, Y)]
End;

Procedure TMap.SetTile(X, Y: NativeInt; T:TTile);
Begin
	_data[Offset(X, Y)] := T
End;

Function TMap.Equals(Const Other: TMap):Boolean;
Var
	Idx: NativeInt;
Begin
	Result := True;
	For Idx := 0 to 24 do
		If Self._data[Idx] <> Other._data[Idx] then begin
			Result := False;
			Break
		end
End;

Procedure TMap.Print();
Var
	Idx: NativeInt;
Begin
	For Idx := 0 to 24 do begin
		If _data[Idx] = TILE_BUG then
			Write('#')
		else
			Write('.');
		
		If ((Idx + 1) mod 5) = 0 then Writeln()
	end;
End;

Function TMap.GetBiodiversity():NativeUInt;
Var
	Idx, Power: NativeInt;
Begin
	Result := 0;

	Power := 1;
	For Idx := 0 to 24 do begin
		If _data[Idx] = TILE_BUG then Result += Power;
		Power *= 2
	end;
End;

Constructor TMap.CreateSimulateOther(Const Other: TMap);
Var
	X, Y: NativeInt;
	Neighbours: NativeInt;
Begin
	For Y := 0 to 4 do
		For X := 0 to 4 do begin
			Neighbours := 0;
			If(Other.GetTile(X,   Y-1) = TILE_BUG) then Neighbours += 1;
			If(Other.GetTile(X+1, Y  ) = TILE_BUG) then Neighbours += 1;
			If(Other.GetTile(X,   Y+1) = TILE_BUG) then Neighbours += 1;
			If(Other.GetTile(X-1, Y  ) = TILE_BUG) then Neighbours += 1;

			If Other.GetTile(X, Y) = TILE_BUG then begin
				If Neighbours <> 1 then
					Self.SetTile(X, Y, TILE_EMPTY)
				else
					Self.SetTile(X, Y, TILE_BUG)
			end else begin
				If (Neighbours = 1) or (Neighbours = 2) then
					Self.SetTile(X, Y, TILE_BUG)
				else
					Self.SetTile(X, Y, TILE_EMPTY)
			end
		end;
End;

Constructor TMap.CreateFromString(Const Str:String);
Var
	Idx: NativeInt;
	T: TTile;
Begin
	For Idx := 0 to 24 do begin
		If Str[Idx+1] = '#' then
			T := TILE_BUG
		else
			T := TILE_EMPTY;
		_data[Idx] := T
	end;
End;

Destructor TMap.Destroy();
Begin
	// nothing to do here
End;


Var
	Current: TMap;
	History: Array of TMap;
	HistCount, HistLength: NativeInt;

Const
	HISTORY_LENGTH_STEP = 4096 div SizeOf(TMap);

Function SeenBefore():Boolean;
Var
	Idx: NativeInt;
Begin
	Result := False;
	For Idx := 0 to (HistCount - 1) do
		If Current.Equals(History[Idx]) then begin
			Result := True;
			Break
		end
End;

Function GetInput():String;
Var
	Idx: NativeInt;
	Line: String;
Begin
	Result := '';
	For Idx := 0 to 4 do begin
		Readln(Line);
		Result += Line
	end
End;

Begin
	HistCount := 0;
	HistLength := HISTORY_LENGTH_STEP;
	SetLength(History, HistLength);

	Current.CreateFromString(GetInput());
	
	Current.Print();
	Writeln();

	While Not SeenBefore() do begin
		If HistCount = HistLength then begin
			HistLength += HISTORY_LENGTH_STEP;
			SetLength(History, HistLength)
		end;

		History[HistCount] := Current;
		Current.CreateSimulateOther(History[HistCount]);
		HistCount += 1;
	
		Current.Print();
		Writeln()	
	end;

	Writeln('Biodiversity rating: ', Current.GetBiodiversity())
End.
