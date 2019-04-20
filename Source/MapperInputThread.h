/*
 ==============================================================================
 
 MapperInputThread.h
 Created: 19 Feb 2019 10:36:05pm
 Author:  johnty
 
 ==============================================================================
 */

#pragma once

#include "mapper/mapper_cpp.h"
#include "JuceHeader.h"
//#include "SigDisplayWindow.h"

#define DEVICE_NAME_STR "inputvis"

class MapperInputThread : public Thread, public ChangeBroadcaster, public ActionBroadcaster
{
    
public:
    MapperInputThread();
    ~MapperInputThread();
    
    void run();
    
    //static database callbacks with pointer to class instances (via final param)
    static void devActionHandler(mapper_database db,
                                 mapper_device dev,
                                 mapper_record_event action,
                                 const void *user);
    static void sigActionHandler(mapper_database db,
                                 mapper_signal sig,
                                 mapper_record_event action,
                                 const void *user);
    static void mapActionHandler(mapper_database db,
                                 mapper_map map,
                                 mapper_record_event action,
                                 const void *user);
    
    //instance database callbacks
    void devActionFn(mapper_device dev,
                     mapper_record_event action) const;
    void sigActionFn(mapper_signal sig,
                     mapper_record_event action) const;
    void mapActionFn(mapper_map map,
                     mapper_record_event action) const;
    
    //static signal update callback
    static void sigUpdateHandler(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag);
    
    //instance signal update callback
    void sigUpdate(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag);
    const double getLastVal() {return lastVal;}
    const double* getLastVals() {return lastVals;}
    const String getLastChangedSigname() {return lastSigName;}
    
    mapper::Device* getDev() {return myMapperDev.get();} //do we have to?
    
private:
    std::unique_ptr<mapper::Device> myMapperDev;
    std::unique_ptr<mapper::Database> myMapperDB;
    //std::vector<mapper::Signal*> myInputSigs;
    
    double lastVal;
    String lastSigName;
    double lastVals[4];
    
    //Array<Component::SafePointer<Component>> myDevWindows;
    
};
