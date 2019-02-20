/*
  ==============================================================================

    MapperInputThread.cpp
    Created: 19 Feb 2019 10:36:05pm
    Author:  johnty

  ==============================================================================
*/

#include "MapperInputThread.h"

MapperInputThread::MapperInputThread(): Thread("MapperInputThread")
{
    myMapperDev = new mapper::Device("inputvis");
    mapper::Signal sig = myMapperDev->add_input_signal("in1", 1, 'f', 0, 0, 0, &MapperInputThread::sigUpdateHandler, this);
    myInputSigs.push_back(sig);
    
    
    myMapperDB.subscribe(MAPPER_OBJ_ALL);
    myMapperDB.add_device_callback(devActionHandler, this);
    myMapperDB.add_signal_callback(sigActionHandler, this);
    myMapperDB.add_map_callback(mapActionHandler, this);
    
}

MapperInputThread::~MapperInputThread()
{
    DBG("stopping mapper thread...\n");
    int res = stopThread(2000);
    DBG("thread stopped. res=" << res);
    
}

void MapperInputThread::run()
{
    DBG(" mapper thread begin\n");
    while (!threadShouldExit())
    {
        myMapperDev->poll(10);
        myMapperDB.poll(10);
    }
    DBG(" mapper thread end\n");
}


void MapperInputThread::devActionHandler(mapper_database db, mapper_device dev,
                            mapper_record_event action,
                            const void *user)
{
    //DBG("static devActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->devActionFn(dev, action);
}


void MapperInputThread::sigActionHandler(mapper_database db, mapper_device dev,
                            mapper_record_event action,
                            const void *user)
{
    //DBG("static sigActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->sigActionFn(dev, action);
}

void MapperInputThread::mapActionHandler(mapper_database db,
                                      mapper_map map,
                                      mapper_record_event action,
                                      const void *user)
{
    //DBG("static mapActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->mapActionFn(map, action);
}


void MapperInputThread::devActionFn(mapper_device dev, mapper_record_event action) const
{
    //DBG("instance dev action!");
}

void MapperInputThread::sigActionFn(mapper_signal sig, mapper_record_event action) const
{
    //DBG("instance sig action!");
}

void MapperInputThread::mapActionFn(mapper_map map, mapper_record_event action) const
{
    //DBG("instance map action!");
}

void MapperInputThread::sigUpdateHandler(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag)
{
    //DBG("static sig update function");
    reinterpret_cast<MapperInputThread*>(mapper::Signal(sig).user_data())->sigUpdate(sig, instance, value, count, timetag);
}

void MapperInputThread::sigUpdate(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag)
{
    DBG("instance sig update function");
    
    if (value) {
        printf("--> destination got %s", mapper_signal_name(sig));
        int len = mapper_signal_length(sig);
        switch (mapper_signal_type(sig)) {
            case 'i': {
                int *v = (int*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %d", v[i]);
                }
                lastVal = (double)v[0];
                DBG(" i; set to "<<lastVal);
                break;
            }
            case 'f': {
                float *v = (float*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %f", v[i]);
                }
                lastVal = (double) v[0];
                DBG(" f; set to "<<lastVal);
            }
            case 'd': {
                double *v = (double*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %f", v[i]);
                }
                lastVal = v[0];
                DBG(" d; set to "<<lastVal);
            }
        }
        printf("\n");

    }
    
    
    sendChangeMessage();
    
}
