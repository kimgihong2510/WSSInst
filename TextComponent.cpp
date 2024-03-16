#include "TextComponent.h"


TextComponent::TextComponent(const juce::String& textToDisplay) : text(textToDisplay)
{
}

void TextComponent::paint(juce::Graphics& g) 
{
    g.setFont(24.0f);
    g.setColour(juce::Colours::white);
    g.drawText(text, getLocalBounds(), juce::Justification::centred, true);
}

void TextComponent::setText(const juce::String& newText)
{
    text = newText;
    repaint();
}
