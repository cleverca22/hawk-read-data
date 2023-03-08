#include "SimpleSerialAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


SimpleSerialAnalyzerSettings::SimpleSerialAnalyzerSettings()
  : mBit0Chan(UNDEFINED_CHANNEL),
  mBit1Chan(UNDEFINED_CHANNEL),
  mSectIdxChan(UNDEFINED_CHANNEL)
{
  mBit0.reset(new AnalyzerSettingInterfaceChannel());
  mBit1.reset(new AnalyzerSettingInterfaceChannel());
  mSectorIndex.reset(new AnalyzerSettingInterfaceChannel());

  mBit0->SetTitleAndTooltip("SA0", "Sector address 0");
  mBit1->SetTitleAndTooltip("SA1", "Sector address 1");
  mSectorIndex->SetTitleAndTooltip("Sector Idx", "Sector index");

  mBit0->SetChannel(mBit0Chan);
  mBit1->SetChannel(mBit1Chan);
  mSectorIndex->SetChannel(mSectIdxChan);

	//mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
	//mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/S)",  "Specify the bit rate in bits per second." );
	//mBitRateInterface->SetMax( 6000000 );
	//mBitRateInterface->SetMin( 1 );
	//mBitRateInterface->SetInteger( mBitRate );

  AddInterface(mBit0.get());
  AddInterface(mBit1.get());
  AddInterface(mSectorIndex.get());
	//AddInterface( mBitRateInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
  AddChannel(mBit0Chan, "Simple Serial1", true );
  AddChannel(mBit1Chan, "Simple Serial2", true );
  AddChannel(mSectIdxChan, "Simple Serial3", true );
}

SimpleSerialAnalyzerSettings::~SimpleSerialAnalyzerSettings()
{
}

bool SimpleSerialAnalyzerSettings::SetSettingsFromInterfaces()
{
  mBit0Chan = mBit0->GetChannel();
  mBit1Chan = mBit1->GetChannel();
  mSectIdxChan = mSectorIndex->GetChannel();

  ClearChannels();
  AddChannel(mBit0Chan, "Simple Serial1", true );
  AddChannel(mBit1Chan, "Simple Serial2", true );
  AddChannel(mSectIdxChan, "Simple Serial3", true );

	return true;
}

void SimpleSerialAnalyzerSettings::UpdateInterfacesFromSettings()
{
	//mBitRateInterface->SetInteger( mBitRate );
  mBit0->SetChannel(mBit0Chan);
  mBit1->SetChannel(mBit1Chan);
  mSectorIndex->SetChannel(mSectIdxChan);
}

void SimpleSerialAnalyzerSettings::LoadSettings( const char* settings ) {
  SimpleArchive text_archive;
  text_archive.SetString( settings );

  text_archive >> mBit0Chan;
  text_archive >> mBit1Chan;
  text_archive >> mSectIdxChan;


  ClearChannels();
  AddChannel(mBit0Chan, "Simple Serial1", true );
  AddChannel(mBit1Chan, "Simple Serial2", true );
  AddChannel(mSectIdxChan, "Simple Serial3", true );

  UpdateInterfacesFromSettings();
}

const char* SimpleSerialAnalyzerSettings::SaveSettings() {
  SimpleArchive text_archive;

  text_archive << mBit0Chan;
  text_archive << mBit1Chan;
  text_archive << mSectIdxChan;

  return SetReturnString( text_archive.GetString() );
}
