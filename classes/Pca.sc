
Pca {
//	
//	var <>rule, <>ruleAsBinary;//base
//	var <>win;//gui
//	var <>nowfs, <>task;//nowfs
//	
//	*new {| list, rule, repeats=inf |
//		^super.newCopyArgs(list, rule, repeats);
//	}
//	storeArgs {^[list, rule, repeats] }
//	//for subclass
//	makerule {}
//	//for subclass
//	evolve {}
//	
//	embedInStream {|inval|
//		this.makerule(rule);
//		repeats.do {
//			inval = this.evolve;
//			inval.yield;
//		}
//		^inval;	
//	}
//}
//
//Pca1 : ListPattern {
//
//	makerule {|char|
//		this.ruleAsBinary = Array.fill(8, {|i| 16r01 & (char >> i) });
//	}
//	evolve {
//		var tmp = list.copy;
//		list.size.do {|i| 
//			list[i] = this.ruleAsBinary[(tmp.wrapAt(i-1)*4) + (tmp[i]*2) + tmp.wrapAt(i+1)] 
//		};
//		^list;
//	}
//
//}
/*
	r1 {
		
		"[Original, Mirrored, Complemental, Mirrored-Complemental]".postln;

			\triangle1 ->	[18, 146] // amphirical
			\triangle2 -> [26, 82, 154, 210]
			\triangle3 -> [22, 151, 129, 161]
			\triangle4 -> [166, 167, 180, 181, 183]
			\triangle5 -> [90, 165]// amphirical
			\gestalt ->	[105, 150] // survive smpwd
			\universal -> [110, 124, 137, 193]
			\harmonic1 -> [60, 102, 195, 153]
			\harmonic2 -> [106, 120, 169, 225] // 120 and 225 survive
			\noise1 -> 	[30, 86, 135, 149], // survive smpwd if init rand
			\noise2 -> 	[122, 126, 146, 182, 192], // wider smpwd conditionally with rand init
			\colored -> 	[45, 75, 89, 101], // wider smpwd init rand
			\pulseflux1 ->[54, 147], // amphirical, no mirror
			\pulseflux2 ->[62, 118, 131, 145],

		motion RL 2, 6, 10, 14, 34, 38, 42, 46, 74, 106
		motion LR 16, 20, 24, 82, 120
		straight 4, 12, 13, 36
	}
*/	
	*r2 {
		//rules for ca2
		//reference, Mirek's MJCell
		//http://psoup.math.wisc.edu/mcell/mjcell/mjcell.html
		^[
		"BC82271C", //bermuda triangle
		"AD9C7232", //chaotic gliders
		"89ED7106", //compound glider
		"1C2A4798",
		"5C6A4D98",
		"5F0C9AD8",
		"B51E9CE8",
		"6C1E53A8",
		"360A96F9",
		"BF8A5CD8",
		"6EA8CD14",
		"4668ED14",
		"D28F022C",
		"6EEAED14",
		"BF8A18C8",
		"BF8A58C8",
		"3CC66B84",
		"3EEE6B84",
		"1D041AC8",
		"5F2A9CC8",
		"1D265EC8",
		"2F8A1858",
		"1D065AD8",
		"BDA258C8",
		"9D041AC8",
		"7E8696DE",
		"978ECEE4",
		"E0897801",
		"8F0C1A48"
		];
	}
}
//	plot2d { | size=1, iter=100 |
//		var rct, cell;
//		
//		rct = Rect(0, 0, smpwd * size, iter * size);
//		win = Window("ca", rct, false);
//		win.view.background = Color.black;
//
//		win.drawHook = {
//			Pen.fillColor = Color.white;
//			iter.do {|i|
//				cell = this.evolve;
//				smpwd.do {|j|
//					if (cell[j]==1, { Pen.fillRect(Rect(j * size, i * size, size, size)) });
//				};
//			};
//
//		};
//		CmdPeriod.doOnce({ win.close });
//		win.front;
//	}
//
//	animate2d { | size=1, iter=100 |
//		var rct, cell, run;
//		
//		rct = Rect(0, 0, smpwd * size, iter * size);
//		win = Window("ca", rct, false);
//		win.view.background = Color.black;
//		run = true;
//		win.onClose = { run = false };
//
//		win.drawHook = {
//			Pen.fillColor = Color.white;
//			iter.do {|i|
//				cell = this.evolve;
//				smpwd.do {|j|
//					if (cell[j]==1, { Pen.fillRect(Rect(j * size, i * size, size, size)) });
//				};
//			};
//		};
//		{ while { run } { win.refresh; 30.reciprocal.wait } }.fork(AppClock);
//		CmdPeriod.doOnce({ win.close });
//		win.front;
//	}
//
//	map2d { | size=1, iter=100, fps=15 |
//		var rct, cell, run, sizeW, sizeH, rndI, rndJ;
//		
//		rct = Rect(0, 0, 640, 480);
//		win = Window("ca", rct, false);
//		win.view.background = Color.black;
//		run = true;
//		win.onClose = { run = false };
//		rndI = Array.series(iter, 0, 1).scramble;
//		rndJ = Array.series(smpwd, 0, 1).scramble;
////		sizeW = Array.geom(smpwd, 1, 1.8).scramble;
////		sizeH = Array.geom(iter, 1, 1.8).scramble;
//		
//		win.drawHook = {
//			Pen.fillColor = Color.white;
//			sizeW = 640 / smpwd;
////			sizeH = 480 / iter;
//			sizeH = 100;
//			iter.do {|i|
//				cell = this.evolve;
//				smpwd.do {|j|
////					if (cell[j]==1, { Pen.fillRect(Rect(rndJ[j] * sizeW[j], rndI[i] * sizeH[i], sizeW[j], sizeH[i])) });
////					if (cell[j]==1, { Pen.fillRect(Rect(j * sizeW, i * sizeH, sizeW, sizeH)) });
//					if (cell[j]==1, { Pen.fillRect(Rect(j * sizeW, 480/2-50, sizeW, sizeH)) });
//				};
//			};
//		};
//		{ while { run } { win.refresh; fps.reciprocal.wait } }.fork(AppClock);
//		CmdPeriod.doOnce({ win.close });
//		win.front;
//	}
//		
//	saveImage {| name='test.png' |
//		var img = SCImage.fromWindow(win, win.view.bounds);
//		img.write("~/Desktop/" ++ name);
//		img.free;
//	}

//
//CA2L : CABase {
//
//	makerule {|hex|
//		hex.do {|item| rule = rule ++ ("16r" ++ item).interpret.asBinaryDigits(4) };
//		rule = rule.reverse;
//	}
//
//	evolve {
//		var tmp = cell.copyRange(0, smpwd - 1);
//		smpwd.do {|i|
//			cell[i] = rule[
//				(cell.wrapAt(i-2) * 16) +
//				(cell.wrapAt(i-1) *  8) + 
//				(cell.at(i)       *  4) + 
//				(cell.wrapAt(i+1) *  2) +
//				(cell.wrapAt(i+2) *  1)
//			];
//		};
//		^cell;
//	}
//	
//}
//
//+ Integer {
//	asCA {|seed=1| ^CA1L(160, this, seed).plot2d(2, 320) } // 160 * 320 resolution
//}
//
//+ String {
//	asCA {|seed=1| ^CA2L(160, this, seed).plot2d(2, 320) } // 160 * 320 resolution
//}
