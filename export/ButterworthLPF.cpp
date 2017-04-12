#include "ButterworthLPF.h"

namespace ButterworthLPF {

/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2016 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing (at) cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	DCBlock __m_dcblock_14;
	DCBlock __m_dcblock_13;
	int __exception;
	int vectorsize;
	t_sample m_yRp_9;
	t_sample m_A_1;
	t_sample m_yRpp_10;
	t_sample m_sum_db_12;
	t_sample m_param_gain_11;
	t_sample m_yLpp_8;
	t_sample m_yLp_7;
	t_sample m_E_5;
	t_sample m_B_2;
	t_sample samplerate;
	t_sample m_C_3;
	t_sample m_D_4;
	t_sample m_Push_6;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_A_1 = ((int)0);
		m_B_2 = ((int)0);
		m_C_3 = ((int)1);
		m_D_4 = ((t_sample)0.5);
		m_E_5 = ((int)0);
		m_Push_6 = ((int)0);
		m_yLp_7 = ((int)0);
		m_yLpp_8 = ((int)0);
		m_yRp_9 = ((int)0);
		m_yRpp_10 = ((int)0);
		m_param_gain_11 = samplerate;
		m_sum_db_12 = ((int)0);
		__m_dcblock_13.reset();
		__m_dcblock_14.reset();
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) {
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) {
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) {
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample speed = m_C_3;
		t_sample res = (((t_sample)0.0014142135623731) * safepow(((int)1000), m_B_2));
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			t_sample xL = in1;
			t_sample xR = in2;
			t_sample gainL = atodb(xL);
			if ((gainL < (-((int)40)))) {
				if ((m_sum_db_12 > ((int)500))) {
					m_param_gain_11 = samplerate;
					
				} else {
					m_sum_db_12 = (m_sum_db_12 + ((int)1));
					
				};
				
			} else {
				m_sum_db_12 = ((int)0);
				
			};
			if ((speed == ((int)0))) {
				m_param_gain_11 = samplerate;
				
			};
			t_sample freq = (m_A_1 * safediv(m_param_gain_11, samplerate));
			t_sample fc = safediv((((int)20) * safepow(((int)1000), freq)), samplerate);
			t_sample a1 = ((((int)1) + (((((int)2) * res) * ((t_sample)3.1415926535898)) * fc)) + ((((int)4) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc)));
			t_sample a2 = (((int)-2) + ((((int)8) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc)));
			t_sample a3 = ((((int)1) - (((((int)2) * res) * ((t_sample)3.1415926535898)) * fc)) + ((((int)4) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc)));
			t_sample b0 = ((((int)4) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc));
			t_sample b1 = ((((int)8) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc));
			t_sample b2 = ((((int)4) * (((t_sample)3.1415926535898) * fc)) * (((t_sample)3.1415926535898) * fc));
			t_sample a_234 = safediv(a2, a1);
			t_sample a_235 = safediv(a3, a1);
			t_sample b_236 = safediv(b0, a1);
			t_sample b_237 = safediv(b1, a1);
			t_sample b_238 = safediv(b2, a1);
			t_sample sum_x_bx_L = (((b_236 * xL) + (b_237 * m_yLp_7)) + (b_238 * m_yLpp_8));
			t_sample sum_x_ay_L = ((a_234 * m_yLp_7) + (a_235 * m_yLpp_8));
			t_sample sum_x_bx_R = (((b_236 * xR) + (b_237 * m_yRp_9)) + (b_238 * m_yRpp_10));
			t_sample sum_x_ay_R = ((a_234 * m_yRp_9) + (a_235 * m_yRpp_10));
			t_sample yL = (sum_x_bx_L - sum_x_ay_L);
			t_sample yR = (sum_x_bx_R - sum_x_ay_R);
			m_yLpp_8 = m_yLp_7;
			m_yLp_7 = yL;
			m_yRpp_10 = m_yRp_9;
			m_yRp_9 = yR;
			m_param_gain_11 = (m_param_gain_11 - speed);
			if ((m_param_gain_11 < ((int)0))) {
				m_param_gain_11 = ((int)0);
				
			};
			t_sample expr_239 = (((((int)1) - m_D_4) * xL) + (m_D_4 * yL));
			t_sample expr_240 = (((((int)1) - m_D_4) * xR) + (m_D_4 * yR));
			t_sample dcblock_2 = __m_dcblock_13(expr_239);
			t_sample out1 = dcblock_2;
			t_sample dcblock_1 = __m_dcblock_14(expr_240);
			t_sample out2 = dcblock_1;
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_A(t_param _value) {
		m_A_1 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_B(t_param _value) {
		m_B_2 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_C(t_param _value) {
		m_C_3 = (_value < 0 ? 0 : (_value > 2 ? 2 : _value));
	};
	inline void set_D(t_param _value) {
		m_D_4 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_E(t_param _value) {
		m_E_5 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_Push(t_param _value) {
		m_Push_6 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	
} State;


///
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs

int gen_kernel_numins = 2;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 6; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1", "in2" };
const char *gen_kernel_outnames[] = { "out1", "out2" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_A(value); break;
		case 1: self->set_B(value); break;
		case 2: self->set_C(value); break;
		case 3: self->set_D(value); break;
		case 4: self->set_E(value); break;
		case 5: self->set_Push(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_A_1; break;
		case 1: *value = self->m_B_2; break;
		case 2: *value = self->m_C_3; break;
		case 3: *value = self->m_D_4; break;
		case 4: *value = self->m_E_5; break;
		case 5: *value = self->m_Push_6; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(6 * sizeof(ParamInfo));
	self->__commonstate.numparams = 6;
	// initialize parameter 0 ("m_A_1")
	pi = self->__commonstate.params + 0;
	pi->name = "A";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_A_1;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_B_2")
	pi = self->__commonstate.params + 1;
	pi->name = "B";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_B_2;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_C_3")
	pi = self->__commonstate.params + 2;
	pi->name = "C";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_C_3;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_D_4")
	pi = self->__commonstate.params + 3;
	pi->name = "D";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_D_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_E_5")
	pi = self->__commonstate.params + 4;
	pi->name = "E";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_E_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_Push_6")
	pi = self->__commonstate.params + 5;
	pi->name = "Push";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_Push_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // ButterworthLPF::
