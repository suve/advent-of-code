Program day21;

{$MODE OBJFPC} {$COPERATORS ON}

Uses
	SysUtils, Images;

Type
	PTransform = ^TTransform;
	TTransform = record
		Input, Output: PImage
	end;

Var
	TransSml: Array of TTransform;
	TransBig: Array of TTransform;

Var
	CurImg, OldImg: PImage;
	Iterations: sInt;

Procedure InitImage();
Const
	StartingPattern: String = '.#./..#/###';
Begin
	New(CurImg, CreateFromPattern(StartingPattern));
	OldImg := NIL
End;

Procedure ProcessArgs();
Begin
	If(ParamCount() < 2) then begin
		Writeln('You must specify the number of iterations: "--iter X"');
		Halt(1)
	end;
	
	If(ParamStr(1) <> '--iter') then begin
		Writeln('Unknown option "', ParamStr(1), '"');
		Halt(1)
	end;
	
	Iterations := StrToInt(ParamStr(2));
	If(Iterations < 0) then begin
		Writeln('Number of iterations must be no less than zero');
		Halt(1)
	end
End;

Procedure ProcessInput();
Var
	Line: AnsiString;
	Len, Idx: sInt;
	trs: TTransform;
Begin
	While Not eof() do begin
		Readln(Line);
		Line := Trim(Line);
		
		Len := Length(Line);
		If(Len = 20) then begin
			New(trs.Input, CreateFromPattern(Copy(Line, 1, 5)));
			New(trs.Output, CreateFromPattern(Copy(Line, 10, 11)));
			
			Idx := Length(TransSml);
			SetLength(TransSml, Idx+1);
			TransSml[Idx] := trs
		end else
		If(Len = 34) then begin
			New(trs.Input, CreateFromPattern(Copy(Line, 1, 11)));
			New(trs.Output, CreateFromPattern(Copy(Line, 16, 19)));
			
			Idx := Length(TransBig);
			SetLength(TransBig, Idx+1);
			TransBig[Idx] := trs
		end else begin
			Writeln('> corrupted input: "', Line, '"');
			Halt(1)
		end;
	end
End;

Procedure Enhance();
Var
	X, Y, step, pp, pf, pr: sInt;
	
	PartOrig: PImage;
	PartMult: Array[0..(4*4)-1] of PImage;
	
	Pattern: PTransform;
	PatternPos, PatternCount: sInt;
	Matched, Transformed: Boolean;
Begin
	If(OldImg <> NIL) then Dispose(OldImg, Destroy());
	
	OldImg := CurImg;
	If((OldImg^.Size mod 2) = 0) then begin
		Pattern := @TransSml[0];
		PatternCount := Length(TransSml);
		step := 2
	end else begin
		Pattern := @TransBig[0];
		PatternCount := Length(TransBig);
		step := 3
	end;
	
	New(CurImg, Create((OldImg^.Size div step) * (step+1)));
	For Y:=0 to (OldImg^.Size div step)-1 do begin
		For X:=0 to (OldImg^.Size div step)-1 do begin
			PartOrig := OldImg^.GetSubImage(X*step, Y*step, step);
			
			New(PartMult[0], CreateCopyOf(PartOrig));
			New(PartMult[4], CreateCopyOf(PartOrig));  PartMult[4]^.FlipHorizontally();
			New(PartMult[8], CreateCopyOf(PartOrig));  PartMult[8]^.FlipVertically();
			New(PartMult[12], CreateCopyOf(PartOrig)); PartMult[12]^.FlipHorizontally(); PartMult[12]^.FlipVertically();
			For pf := 0 to 3 do begin
				For pr := 1 to 3 do begin
					New(PartMult[pf*4 + pr], CreateCopyOf(PartMult[pf*4 + (pr-1)]));
					PartMult[pf*4 + pr]^.RotateClockwise()
				end
			end;
			
			Transformed := False;
			
			For PatternPos:=0 to PatternCount-1 do begin
				//Writeln('Comparing: "', Part^.ToPatternString,'" vs "',Pattern[PatternPos].Input^.ToPatternString(),'"'); 
				
				Matched := False;
				For pp:=0 to 15 do If(Not Matched) then Matched:=PartMult[pp]^.Equals(Pattern[PatternPos].Input);
				
				If(Matched) then begin
					CurImg^.SetSubImage(X*(step+1), Y*(step+1), Pattern[PatternPos].Output);
					Transformed := True;
					Break
				end
			end;
			
			If(Not Transformed) then begin
				Writeln('No match found for ', PartOrig^.ToPatternString());
				Halt(1)
			end;
			
			For pp:=0 to 15 do Dispose(PartMult[pp], Destroy());
			Dispose(PartOrig, Destroy());
		end
	end
End;

Begin
	ProcessArgs();
	
	ProcessInput();
	Writeln('> 2x2->3x3 patterns: ', Length(TransSml));
	Writeln('> 3x3->4x4 patterns: ', Length(TransBig));
	
	InitImage();
	Writeln('> Simulation start; ', CurImg^.PixelsLit,' pixels lit');
	Write(CurImg^.ToPrettyString());
	
	While(Iterations > 0) do begin
		Enhance();
		
		Writeln('> Iteration complete; ', CurImg^.PixelsLit,' pixels lit');
		Write(CurImg^.ToPrettyString());
		
		Iterations -= 1
	end;
	
	If(OldImg <> NIL) then Dispose(OldImg, Destroy());
	If(CurImg <> NIL) then Dispose(CurImg, Destroy())
End.
