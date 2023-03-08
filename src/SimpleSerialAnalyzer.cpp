#include "SimpleSerialAnalyzer.h"
#include "SimpleSerialAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

SimpleSerialAnalyzer::SimpleSerialAnalyzer()
:	Analyzer2(),  
	mSettings( new SimpleSerialAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

SimpleSerialAnalyzer::~SimpleSerialAnalyzer()
{
	KillThread();
}

void SimpleSerialAnalyzer::SetupResults()
{
	mResults.reset( new SimpleSerialAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mBit0Chan );
}

void SimpleSerialAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mSectorIdx = GetAnalyzerChannelData( mSettings->mSectIdxChan );
  mBit0 = GetAnalyzerChannelData(mSettings->mBit0Chan);
  mBit1 = GetAnalyzerChannelData(mSettings->mBit1Chan);

	if( mSectorIdx->GetBitState() == BIT_LOW )
		mSectorIdx->AdvanceToNextEdge();

	//U32 samples_per_bit = mSampleRateHz / mSettings->mBitRate;
	//U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( mSettings->mBitRate ) );


  for( ; ; )  {
    mSectorIdx->AdvanceToNextEdge(); //falling edge -- start of sector

    U64 starting_sample = mSectorIdx->GetSampleNumber();

    //mSerial->Advance( samples_to_first_center_of_first_data_bit );

    mBit0->AdvanceToAbsPosition(starting_sample);
    mBit1->AdvanceToAbsPosition(starting_sample);
    uint8_t sa = 0;
    if (mBit0->GetBitState() == BIT_LOW) sa |= 1;
    if (mBit1->GetBitState() == BIT_LOW) sa |= 2;

    mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mBit0Chan);
    mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mBit1Chan);

    Frame frame;
    frame.mData1 = sa;
    frame.mFlags = 0;
    frame.mStartingSampleInclusive = starting_sample;

    mSectorIdx->AdvanceToNextEdge();
    frame.mEndingSampleInclusive = mSectorIdx->GetSampleOfNextEdge();

    mResults->AddFrame(frame);
    mResults->CommitResults();
    ReportProgress(frame.mEndingSampleInclusive);
  }
}

bool SimpleSerialAnalyzer::NeedsRerun()
{
	return false;
}

U32 SimpleSerialAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		//mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}
        return 0;

	//return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SimpleSerialAnalyzer::GetMinimumSampleRateHz()
{
  return 100000;
}

const char* SimpleSerialAnalyzer::GetAnalyzerName() const
{
	return "Hawk - Sector Addr 2";
}

const char* GetAnalyzerName()
{
	return "Hawk - Sector Addr";
}

Analyzer* CreateAnalyzer()
{
	return new SimpleSerialAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
