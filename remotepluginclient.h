/* -*- c-basic-offset: 4 -*- */

/*
  dssi-vst: a DSSI plugin wrapper for VST effects and instruments
  Copyright 2012-2013 Filipe Coelho
  Copyright 2010-2011 Kristian Amlie
  Copyright 2004-2010 Chris Cannam
*/

#ifndef REMOTE_PLUGIN_CLIENT_H
#define REMOTE_PLUGIN_CLIENT_H

#include "remoteplugin.h"
#include "rdwrops.h"
#include <string>
#include <vector>
#include <sys/shm.h>

// Any of the methods in this file, including constructors, should be
// considered capable of throwing RemotePluginClosedException.  Do not
// call anything here without catching it.

class RemotePluginClient
{
public:
    virtual ~RemotePluginClient();

    std::string  getFileIdentifiers();

    float        getVersion();
    std::string  getName();
    std::string  getMaker();

    void         setBufferSize(int);
    void         setSampleRate(int);

    void         reset();
    void         terminate();
    
    int          getInputCount();
    int          getOutputCount();

    int          getParameterCount();
    std::string  getParameterName(int);
    void         setParameter(int, float);
    float        getParameter(int);
    float        getParameterDefault(int);
    void         getParameters(int, int, float *);

    int          getProgramCount();
    std::string  getProgramName(int);
    void         setCurrentProgram(int);

    bool         hasMIDIInput();

    // Must be three bytes per event
    void         sendMIDIData(unsigned char *data, int *frameoffsets, int events);

    // Either inputs or outputs may be NULL if (and only if) there are none
    void         process(float **inputs, float **outputs);

    void         waitForServer();

    void         setDebugLevel(RemotePluginDebugLevel);
    bool         warn(std::string);

    void         showGUI(std::string guiData);
    void         hideGUI();

    //Deryabin Andrew: vst chunks support
    std::vector<char> getVSTChunk();
    void              setVSTChunk(std::vector<char> chunk);
    //Deryabin Andrew: vst chunks support: end code

protected:
    RemotePluginClient();

    void         cleanup();
    void         syncStartup();

private:
    RemotePluginClient(const RemotePluginClient &); // not provided
    RemotePluginClient &operator=(const RemotePluginClient &); // not provided

    int m_controlRequestFd;
    int m_controlResponseFd;
    int m_shmFd;
    int m_shmControlFd;

    char *m_controlRequestFileName;
    char *m_controlResponseFileName;
    char *m_shmFileName;
    char *m_shmControlFileName;

    char *m_shm;
    size_t m_shmSize;
    ShmControl *m_shmControl;

    int m_bufferSize;
    int m_numInputs;
    int m_numOutputs;

    void sizeShm();
};


#endif

