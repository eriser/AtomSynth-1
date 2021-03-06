#ifndef ATOM_FirLowpass_HEADER
#define ATOM_FirLowpass_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include <array>

#include "Adsp/CachedFir.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

/**
* YOUR DOCUMENTATION HERE
*/
class FirLowpassController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	MultiButton m_cutoffSource;
	SemitonesKnob m_semis;
	DVecIter m_semisIter;
	OctavesKnob m_octs;
	DVecIter m_octsIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	FirLowpassController();
	virtual AtomController * createNewInstance() {
		return new FirLowpassController();
	}
	virtual ~FirLowpassController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Filters";
	}
	virtual std::string getName() {
		return "Lowpass (FIR)";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void multiButtonPressed(MultiButton * button);
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class FirLowpassAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class FirLowpassAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	FirLowpassController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	static constexpr int FILTER_TYPE = Adsp::FilterType::BLACKMAN | Adsp::FilterType::LOWPASS;
	Adsp::CachedFirFilter m_filter;
	AudioBuffer m_delayLine;

	void recalculate(double newFreq);
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	FirLowpassAtom(FirLowpassController & parent, int index);
	virtual ~FirLowpassAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class FirLowpassController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_FirLowpass_HEADER */

