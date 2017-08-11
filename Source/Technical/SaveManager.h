/*
 * SaveManager.h
 *
 *  Created on: Aug 10, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_SAVEMANAGER_H_
#define SOURCE_TECHNICAL_SAVEMANAGER_H_

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

namespace AtomSynth {

class Synth;
class SaveManager;

/**
 * A worker thread for SaveManager which calls
 * SaveManager::saveNow() whenever SaveManager
 * is scheduled to do an autosave.
 */
class AutosaveThread: public Thread {
private:
	SaveManager *m_parent;
public:
	AutosaveThread(SaveManager *parent);
	virtual ~AutosaveThread();

	virtual void run();
};

/**
 * Manages saving and loading of the synth,
 * as well as the properties of the synth
 * associated with it, such as the name of
 * the patch.
 */
class SaveManager {
private:
	std::string m_name = "Unnamed";
	int m_autosaveRevision = 0; ///< Used when the user has not specified a name yet (saves files like autosave-1, autosave-2, etc.)
	static constexpr int MAX_AUTOSAVE_REVISIONS = 4; ///< m_autosaveRevision should loop back to 0 before hitting this number.
	unsigned int m_nextAutosave = 2, ///< When the next autosave will occur.
			m_lastAutosave = 0;
	AutosaveThread m_autosaveThread;
	Synth * m_parent = nullptr;
	void setup();
	unsigned int getTime(); ///< The c++ time library is a mess, I just want milliseconds!

	friend class Synth;
	friend class AutosaveThread;
public:
	SaveManager();
	virtual ~SaveManager();

	/**
	 * Schedules an autosave to occur a number
	 * of milliseconds in the future.
	 * @param millisDelay How many milliseconds before the autosave occurrs.
	 */
	void scheduleAutosave(unsigned int millisDelay = 30 * 1000) {
		m_nextAutosave = getTime() + millisDelay;
	}
	/**
	 * Sets the name of the current patch, and
	 * schedules an autosave to write to the new
	 * file name.
	 * @param newName What the synth should be called.
	 */
	void setPatchName(std::string newName);
	/**
	 * Returns the name of the current patch.
	 * @return The name of the current patch.
	 */
	std::string getPatchName() {
		return m_name;
	}
	/**
	 * Forcibly saves the synthesizer to a file,
	 * and resets the autosave timer to its default
	 * value.
	 */
	void saveNow();
	/**
	 * Loads a patch given its name. (E.G. 'patch1'
	 * will load the file 'patch1.mol'.)
	 * @param loadFrom A Juce file object pointing to the file that should be loaded from.
	 * @return False if there was a problem loading the file.
	 */
	bool load(File loadFrom);
	/**
	 * Returns a list of patch names that can be loaded
	 * via load().
	 * @return A list of available patches.
	 */
	std::vector<std::string> getAvailablePatches();
	/**
	 * Returns the number of milliseconds that have
	 * passed since the last autosave was performed.
	 * @return The number of milliseconds since the last autosave.
	 */
	unsigned int getTimeSinceSave() {
		return getTime() - m_lastAutosave;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_SAVEMANAGER_H_ */
