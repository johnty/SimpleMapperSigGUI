/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MapperInputThread.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component, public ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    ScopedPointer<MapperInputThread> myMapperInput;
    
    Slider sigSlider;
    Slider sigSlider1;
    Slider sigSlider2;
    Slider sigSlider3;
    Label sigLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
