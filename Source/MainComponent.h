/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MapperInputThread.h"
#include "SigDisplayWindow.h"
#include "mapper/mapper_cpp.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component, public ChangeListener, public ActionListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void actionListenerCallback(const String& message) override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    ScopedPointer<MapperInputThread> myMapperInput;
    ScopedPointer<SigDisplayWindow> mySigDisplay;
    
    Array<Component::SafePointer<Component>> windows;
    
    std::vector<NamedValueSet> mySigVals;
    
    std::vector<Slider> mySigSliders;
    
    Slider sigSlider;
    Slider sigSlider1;
    Slider sigSlider2;
    Slider sigSlider3;
    Label sigLabel;
    
    ComboBox comboBox  { "Options" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
