
CAUGen : UGen {
	*categories {^#["UGens>ca4sc"] }
}

CA0 : CAUGen {
	*ar {| smprt=22050, smpwd=32, rule=18, seed=1, trig=0, mul=1.0, add=0.0 |
		^this.multiNew(\audio, smprt, smpwd, rule, seed, trig).madd(mul, add);
	}
}
CA0x : CAUGen {
	*ar {| smprt=22050, smpwd=32, rule=18, seed=1, trig=0, mul=1.0, add=0.0 |
		^this.multiNew(\audio, smprt, smpwd, rule, seed, trig).madd(mul, add);
	}
}

CA1 : CAUGen {
	*ar {| smprt=22050, smpwd=60, rule=18, seed=1, trig=0, mul=1.0, add=0.0 |
		^this.multiNew(\audio, smprt, smpwd, rule, seed, trig).madd(mul, add);
	}
}
CA1x : CAUGen {
	*ar {| smprt=22050, smpwd=60, rule=18, seed=1, trig=0, mul=1.0, add=0.0 |
		^this.multiNew(\audio, smprt, smpwd, rule, seed, trig).madd(mul, add);
	}
}

CA2 : CAUGen {
	*ar {| smprt=22050, smpwd, rule="1C2A4798", seed=1, trig=0, mul=1.0, add=0.0 |
		var ruleArray = Array.newClear( 8 );		
		rule.do {|item, i| ruleArray[i] = ("16r" ++ item).interpret.asBinaryDigits(4) };
		ruleArray = ruleArray.flat.reverse;
		^this.multiNewList([\audio, smprt, smpwd, seed, trig] ++ ruleArray);
	}
}
CA2x : CAUGen {
	*ar {| smprt=22050, smpwd, rule="1C2A4798", seed=1, trig=0, mul=1.0, add=0.0 |
		var ruleArray = Array.newClear( 8 );		
		rule.do {|item, i| ruleArray[i] = ("16r" ++ item).interpret.asBinaryDigits(4) };
		ruleArray = ruleArray.flat.reverse;
		^this.multiNewList([\audio, smprt, smpwd, seed, trig] ++ ruleArray);
	}
}
