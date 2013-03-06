/*
 SuperCollider real time audio synthesis system
 Copyright (c) 2002 James McCartney. All rights reserved.
 http://www.audiosynth.com
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

// NLUGens by yota morimoto (http://yota.tehis.net/)
// v1.7 2013
// CA1(x)
// CA2(x)
// x = cubic interpolation

#include "SC_PlugIn.h"

#define MAXWIDTH 256

static InterfaceTable *ft;

struct CA0 : public Unit {
	unsigned char rule[8];
	unsigned char rule2[32];
	unsigned char cell[MAXWIDTH];
	float counter;
	float xn, xm1, xm2, xm3, frac, c0, c1, c2, c3;
};
struct CA1  : public CA0 {};
struct CA2  : public CA0 {};
struct CA1x : public CA0 {};
struct CA2x : public CA0 {};

extern "C" {
	void CA1_next(CA1 *unit, int inNumSamples);
	void CA1_Ctor(CA1 *unit);
	void CA2_next(CA2 *unit, int inNumSamples);
	void CA2_Ctor(CA2 *unit);
	void CA1x_next(CA1x *unit, int inNumSamples);
	void CA1x_Ctor(CA1x *unit);
	void CA2x_next(CA2x *unit, int inNumSamples);
	void CA2x_Ctor(CA2x *unit);
}

// calc 3rd order interpolation coefs from four points
static inline void ipol3Coef(float xm3, float xm2, float xm1, float x, float &c0, float &c1, float &c2, float &c3){
	c0 = xm2;
	c1 = 0.5f * (xm1 - xm3);
	c2 = xm3 - (2.5f * xm2) + xm1 + xm1 - 0.5f * x;
	c3 = 0.5f * (x - xm3) + 1.5f * (xm2 - xm1);
}

// do 3rd order interpolation using coefs
static inline float ipol3(float frac, float c0, float c1, float c2, float c3){
	return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

float CA1_evolve(CA1 *unit, unsigned char smpwd);
float CA1_evolve(CA1 *unit, unsigned char smpwd)
{
	float result, sign, div;
	
	unsigned char tmp[MAXWIDTH];
	memcpy(tmp, unit->cell, sizeof(unit->cell));
	
	result = 0.f;
	sign = 1.f - 2.f * tmp[0];
	div = 1;

	for (int i=0; i<smpwd; i++) {
		unit->cell[i] = unit->rule[
			tmp[sc_wrap(i-1, 0, MAXWIDTH-1)] * 4 + 
			tmp[i]							 * 2 + 
			tmp[sc_wrap(i+1, 0, MAXWIDTH-1)]
		];
		div += i;
		result += tmp[i] * i;
	}
	return sign * result / div;
}

void CA1_next(CA1 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);
	
	float xn = unit->xn;
	float counter = unit->counter;
	
	// rexcitation
	if (IN0(4)) {
		if (IN0(3)) {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%128%2;
		}
	}
	
	float spc;
	if (smprt < SAMPLERATE)
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
	else spc = 1.f;
	
	LOOP(inNumSamples,
		if (counter >= spc){
			counter -= spc;
			xn = CA1_evolve(unit, smpwd);
		}
		counter++;
		ZXP(out) = xn;
	)
	unit->counter = counter;
	unit->xn = xn;
}

void CA1_Ctor(CA1 *unit)
{
	unit->xn = 0.f;
	for (int i=0; i<8; i++)
		unit->rule[i] = 0x01 & ((unsigned char)IN0(2) >> i);

	if (IN0(3)) {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
	} else {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = rand()%2;
	}
	SETCALC(CA1_next);
	unit->counter = 0.f;	
}

void CA1x_next(CA1x *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);

	float xn = unit->xn;
	float xm1 = unit->xm1;
	float xm2 = unit->xm2;
	float xm3 = unit->xm3;
	float c0 = unit->c0;
	float c1 = unit->c1;
	float c2 = unit->c2;
	float c3 = unit->c3;
		
	float counter = unit->counter;
	float frac = unit->frac;	

	float spc;
	float slope;
	if(smprt < SAMPLERATE){
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
		slope = 1.f / spc;
	} 
	else spc = slope = 1.f;

	// rexcitation
	if (IN0(4)) {
		if (IN0(3)) {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%128%2;
		}
	}
	
	LOOP(inNumSamples,
		if (counter >= spc){
			counter -= spc;
			frac = 0.f;
			xm3 = xm2;
			xm2 = xm1;
			xm1 = xn;
			xn = CA1_evolve((CA1*)unit, smpwd);
			ipol3Coef(xm3, xm2, xm1, xn, c0, c1, c2, c3);
		}
		counter++;
		ZXP(out) = ipol3(frac, c0, c1, c2, c3);
		frac += slope;
	)
	unit->xn = xn;
	unit->counter = counter;
	unit->frac = frac;
	unit->xm1 = xm1;
	unit->xm2 = xm2;
	unit->xm3 = xm3;
	unit->c0 = c0;
	unit->c1 = c1;
	unit->c2 = c2;
	unit->c3 = c3;
}

void CA1x_Ctor(CA1x *unit)
{
	//RGET;
	unit->xn = unit->xm1 = unit->xm2 = unit->xm3 = 0.f;
	unit->c0 = unit->c1 = unit->c2 = unit->c3 = 0.f;
	
	for (int i=0; i<8; i++)
		unit->rule[i] = 0x01 & ((unsigned char)IN0(2) >> i);

	if (IN0(3)) {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
	} else {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = rand()%2;//rgen.irand(2);
	}
	unit->counter = 0.f;
	unit->frac = 0.f;
	SETCALC(CA1x_next);
}

float CA2_evolve(CA2 *unit, unsigned char smpwd);
float CA2_evolve(CA2 *unit, unsigned char smpwd)
{
	float result, sign, div;
	
	unsigned char tmp[MAXWIDTH];
	memcpy(tmp, unit->cell, sizeof(unit->cell));

	result = 0.f;
	sign = 1.f - 2.f * tmp[0];
	div = 1;

	for (int i=0; i<smpwd; i++) {
		unit->cell[i] = unit->rule2[
			tmp[sc_wrap(i-2, 0, MAXWIDTH-1)]		* 16 +
			tmp[sc_wrap(i-1, 0, MAXWIDTH-1)]		*  8 +
			tmp[i]									*  4 + 
			tmp[sc_wrap(i+1, 0, MAXWIDTH-1)]		*  2 + 
			tmp[sc_wrap(i+2, 0, MAXWIDTH-1)]
		];		
		result += tmp[i] * i;
		div += i;
	}
	return sign * result / div;
}

void CA2_next(CA2 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);
	
	float xn = unit->xn;
	float counter = unit->counter;
	
	// rexcitation
	if (IN0(3)) {
		if (IN0(2)) {
			for(int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(2) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%2;
		}
	}
	
	float spc;
	if (smprt < SAMPLERATE)
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
	else spc = 1.f;

	LOOP(inNumSamples,
		if(counter >= spc){
			counter -= spc;
			xn = CA2_evolve(unit, smpwd);
		}
		counter++;
		ZXP(out) = xn
	)
	unit->counter = counter;
	unit->xn = xn;
}

void CA2_Ctor(CA2 *unit)
{
	unit->xn = 0.f;
	for(int i=0; i<31; i++)
		unit->rule2[i] = (bool)IN0(i+4); // create rule

	for(int i=0; i<MAXWIDTH; i++)
		unit->cell[i] = rand()%2;

	SETCALC(CA2_next);
	unit->counter = 0.f;
}

void CA2x_next(CA2x *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);
	
	float xn = unit->xn;
	float xm1 = unit->xm1;
	float xm2 = unit->xm2;
	float xm3 = unit->xm3;
	float c0 = unit->c0;
	float c1 = unit->c1;
	float c2 = unit->c2;
	float c3 = unit->c3;
		
	float counter = unit->counter;
	float frac = unit->frac;
	
	// rexcitation
	if (IN0(3)) {
		if (IN0(2)) {
			for(int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(2) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%2;
		}
	}
	
	float spc;
	float slope;
	if(smprt < SAMPLERATE){
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
		slope = 1.f / spc;
	} 
	else spc = slope = 1.f;

	LOOP(inNumSamples,
		if(counter >= spc){
			counter -= spc;
			frac = 0.f;
			xm3 = xm2;
			xm2 = xm1;
			xm1 = xn;
			xn = CA2_evolve((CA2*)unit, smpwd);
			ipol3Coef(xm3, xm2, xm1, xn, c0, c1, c2, c3);
		}
		counter++;
		ZXP(out) = ipol3(frac, c0, c1, c2, c3);
		frac += slope;
	)
	unit->xn = xn;
	unit->counter = counter;
	unit->frac = frac;
	unit->xm1 = xm1;
	unit->xm2 = xm2;
	unit->xm3 = xm3;
	unit->c0 = c0;
	unit->c1 = c1;
	unit->c2 = c2;
	unit->c3 = c3;
}

void CA2x_Ctor(CA2x *unit)
{
	unit->xn = unit->xm1 = unit->xm2 = unit->xm3 = 0.f;
	unit->c0 = unit->c1 = unit->c2 = unit->c3 = 0.f;
	for(int i=0; i<31; i++)
		unit->rule2[i] = (unsigned char)IN0(i+4); // create rule

	if(IN0(2))
		for(int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = 0x01 & ((unsigned char)IN0(2) >> i%8);
	else 
		for(int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = rand()%2;
	unit->counter = 0.f;
	unit->frac = 0.f;
	SETCALC(CA2x_next);
}

PluginLoad(CA)
{
	ft = inTable;
	DefineSimpleUnit(CA1);
	DefineSimpleUnit(CA2);
	DefineSimpleUnit(CA1x);
	DefineSimpleUnit(CA2x);
}
