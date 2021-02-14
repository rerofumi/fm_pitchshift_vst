/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginView.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void PluginView::setProcesser(Fm_pitchshift_vstAudioProcessor& p)
{
    processer = &p;
}
//[/MiscUserDefs]

//==============================================================================
PluginView::PluginView ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    PitchSlider.reset (new juce::Slider ("PitchShift"));
    addAndMakeVisible (PitchSlider.get());
    PitchSlider->setTooltip (TRANS("Pitch"));
    PitchSlider->setRange (1, 2, 0);
    PitchSlider->setSliderStyle (juce::Slider::LinearHorizontal);
    PitchSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, true, 80, 20);
    PitchSlider->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff2d2308));
    PitchSlider->setColour (juce::Slider::thumbColourId, juce::Colours::coral);
    PitchSlider->addListener (this);

    PitchSlider->setBounds (16, 280, 504, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PluginView::~PluginView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    PitchSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginView::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 12, y = 12, width = 512, height = 256;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        g.drawImage(processer->getSpectrumView(), x, y, width, height, 0, 0, width, height);
        //[/UserPaintCustomArguments]
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PluginView::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == PitchSlider.get())
    {
        //[UserSliderCode_PitchSlider] -- add your slider handling code here..
        processer->setPitchShift((float)PitchSlider.get()->getValue());
        //[/UserSliderCode_PitchSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginView" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <IMAGE pos="12 12 512 256" resource="" opacity="1.0" mode="0"/>
  </BACKGROUND>
  <SLIDER name="PitchShift" id="e45884d728ca3120" memberName="PitchSlider"
          virtualName="" explicitFocusOrder="0" pos="16 280 504 24" tooltip="Pitch"
          bkgcol="ff2d2308" thumbcol="ffff7f50" min="1.0" max="2.0" int="0.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

