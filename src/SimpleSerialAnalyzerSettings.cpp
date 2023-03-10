#include "SimpleSerialAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


SimpleSerialAnalyzerSettings::SimpleSerialAnalyzerSettings()
  : mReadClockChan(UNDEFINED_CHANNEL),
  mReadDataChan(UNDEFINED_CHANNEL),
  mReadGateChan(UNDEFINED_CHANNEL)
{
  mReadClock.reset(new AnalyzerSettingInterfaceChannel());
  mReadData.reset(new AnalyzerSettingInterfaceChannel());
  mReadGate.reset(new AnalyzerSettingInterfaceChannel());

  mReadClock->SetTitleAndTooltip("Read Clock", "Hawk read clock");
  mReadData->SetTitleAndTooltip("Read Data", "Hawk read data");
  mReadGate->SetTitleAndTooltip("Read Gate", "Hawk read gate");

  mReadClock->SetChannel(mReadClockChan);
  mReadData->SetChannel(mReadDataChan);
  mReadGate->SetChannel(mReadGateChan);

	//mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
	//mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/S)",  "Specify the bit rate in bits per second." );
	//mBitRateInterface->SetMax( 6000000 );
	//mBitRateInterface->SetMin( 1 );
	//mBitRateInterface->SetInteger( mBitRate );

  AddInterface(mReadClock.get());
  AddInterface(mReadData.get());
  AddInterface(mReadGate.get());
	//AddInterface( mBitRateInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

  ClearChannels();
  AddChannel(mReadClockChan, "Hawk read clock", true);
  AddChannel(mReadDataChan, "Hawk read data", true);
  AddChannel(mReadGateChan, "Hawk read gate", true);
}

SimpleSerialAnalyzerSettings::~SimpleSerialAnalyzerSettings()
{
}

bool SimpleSerialAnalyzerSettings::SetSettingsFromInterfaces()
{
  mReadClockChan = mReadClock->GetChannel();
  mReadDataChan = mReadData->GetChannel();
  mReadGateChan = mReadGate->GetChannel();

  ClearChannels();
  AddChannel(mReadClockChan, "Hawk read clock", true);
  AddChannel(mReadDataChan, "Hawk read data", true);
  AddChannel(mReadGateChan, "Hawk read gate", true);

  return true;
}

void SimpleSerialAnalyzerSettings::UpdateInterfacesFromSettings()
{
  //mBitRateInterface->SetInteger( mBitRate );
  mReadClock->SetChannel(mReadClockChan);
  mReadData->SetChannel(mReadDataChan);
  mReadGate->SetChannel(mReadGateChan);
}

void SimpleSerialAnalyzerSettings::LoadSettings( const char* settings ) {
  SimpleArchive text_archive;
  text_archive.SetString( settings );

  text_archive >> mReadClockChan;
  text_archive >> mReadDataChan;
  text_archive >> mReadGateChan;

  ClearChannels();
  AddChannel(mReadClockChan, "Hawk read clock", true);
  AddChannel(mReadDataChan, "Hawk read data", true);
  AddChannel(mReadGateChan, "Hawk read gate", true);

  UpdateInterfacesFromSettings();
}

const char* SimpleSerialAnalyzerSettings::SaveSettings() {
  SimpleArchive text_archive;

  text_archive << mReadClockChan;
  text_archive << mReadDataChan;
  text_archive << mReadGateChan;

  return SetReturnString( text_archive.GetString() );
}
