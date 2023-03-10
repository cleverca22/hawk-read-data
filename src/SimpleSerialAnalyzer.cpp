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

void SimpleSerialAnalyzer::SetupResults() {
  mResults.reset( new SimpleSerialAnalyzerResults( this, mSettings.get() ) );
  SetAnalyzerResults( mResults.get() );
  mResults->AddChannelBubblesWillAppearOn( mSettings->mReadDataChan );
}

void SimpleSerialAnalyzer::WorkerThread() {
  mSampleRateHz = GetSampleRate();

  mReadClock = GetAnalyzerChannelData(mSettings->mReadClockChan);
  mReadData = GetAnalyzerChannelData(mSettings->mReadDataChan);
  mReadGate = GetAnalyzerChannelData(mSettings->mReadGateChan);

  if( mReadClock->GetBitState() == BIT_LOW ) mReadClock->AdvanceToNextEdge();

	//U32 samples_per_bit = mSampleRateHz / mSettings->mBitRate;
	//U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( mSettings->mBitRate ) );

  bool waiting_for_sync_end = true;
  U64 sync_start = 0;
  int bits = 0;
  uint16_t byte = 0;
  U64 byte_start = 0;
  bool addr = true;

  mReadClock->AdvanceToNextEdge(); //falling edge -- start of bit
  for( ; ; )  {
    U64 starting_sample = mReadClock->GetSampleNumber();
    if (sync_start == 0) sync_start = starting_sample;
    if (byte_start == 0) byte_start = starting_sample;

    mReadClock->AdvanceToNextEdge(); // rising edge
    U64 rising_edge = mReadClock->GetSampleNumber();

    mReadClock->AdvanceToNextEdge(); // falling edge
    U64 next_start = mReadClock->GetSampleNumber();

    U64 bit_length_samples = next_start - starting_sample;
    double bit_length = (((double)bit_length_samples) / mSampleRateHz) * 1000 * 1000 * 1000;

    mReadGate->AdvanceToAbsPosition(next_start);
    if ((mReadGate->GetBitState() == BIT_HIGH) || (bit_length > 800)) {
      waiting_for_sync_end = true;
      sync_start = 0;
      byte_start = 0;
      addr = !addr;
      continue;
    }

    U64 sample_point = rising_edge + ((next_start - rising_edge)/2);

    mReadData->AdvanceToAbsPosition(sample_point);

    bool bit = mReadData->GetBitState() == BIT_LOW;

    if (!waiting_for_sync_end) {
      if (addr) {
        if (bit) {
          byte = (byte >> 1) | 0x8000;
        } else {
          byte = (byte >> 1);
        }
      } else {
        if (bit) {
          byte = (byte << 1) | 0x1;
        } else {
          byte = (byte << 1);
        }
      }
      bits++;
      if ((addr && (bits == 16) || (!addr && (bits == 8)))) {
        Frame f;
        f.mData1 = byte;
        //if (!addr) f.mData1 &= 0x7f;
        f.mFlags = 0;
        f.mStartingSampleInclusive = byte_start;
        f.mEndingSampleInclusive = sample_point;
        //if (addr) {
          mResults->AddFrame(f);
          mResults->CommitResults();
        //}
        bits = 0;
        byte = 0;
        byte_start = 0;
      }
    }

    if (bit && waiting_for_sync_end) {
      // end of sync
      //mResults->GenerateBubbleText(sample_point, mSettings->mReadDataChan, 
      //FrameV2 f;
      //f.AddString("symbol", "sync");
      //mResults->AddFrameV2(f, "symbol", sync_start, sample_point);
      //mResults->CommitResults();
      waiting_for_sync_end = false;
      bits = 0;
      byte = 0;
      byte_start = 0;
    }

    enum AnalyzerResults::MarkerType m = bit ? AnalyzerResults::One : AnalyzerResults::Zero;
    mResults->AddMarker(sample_point, m, mSettings->mReadDataChan);

    //mReadClock->AdvanceToAbsPosition(next_start - 2);

    //mSerial->Advance( samples_to_first_center_of_first_data_bit );

    Frame frame;
    frame.mData1 = bit;
    frame.mFlags = 0;
    frame.mStartingSampleInclusive = starting_sample;

    frame.mEndingSampleInclusive = next_start;

    //mResults->AddFrame(frame);
    //mResults->CommitResults();
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
  return 5000000;
}

const char* SimpleSerialAnalyzer::GetAnalyzerName() const
{
	return "Hawk - Read data 2";
}

const char* GetAnalyzerName()
{
	return "Hawk - Read data";
}

Analyzer* CreateAnalyzer()
{
	return new SimpleSerialAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
