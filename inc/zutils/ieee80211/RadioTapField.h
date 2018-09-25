/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __IEEE80211_RADIOTAPFIELD_H__
#define __IEEE80211_RADIOTAPFIELD_H__

#include <stdint.h>

#include <vector>
#include <map>

#include <zutils/zSocket.h>
using namespace zUtils;

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: RadioTapField
//*****************************************************************************

class RadioTapField
{
public:

  enum ID
  {
    ID_ERR = -1,
    ID_TSFT = 0,
    ID_FLAGS = 1,
    ID_RATE = 2,
    ID_CHANNEL = 3,
    ID_FHSS = 4,
    ID_DBM_ANTSIGNAL = 5,
    ID_DBM_ANTNOISE = 6,
    ID_LOCK_QUALITY = 7,
    ID_TX_ATTENUATION = 8,
    ID_DB_TX_ATTENUATION = 9,
    ID_DBM_TX_POWER = 10,
    ID_ANTENNA = 11,
    ID_DB_ANTSIGNAL = 12,
    ID_DB_ANTNOISE = 13,
    ID_RX_FLAGS = 14,
    ID_TX_FLAGS = 15,
    ID_RTS_RETRIES = 16,
    ID_DATA_RETRIES = 17,
    ID_RSVD1 = 18,
    ID_MCS = 19,
    ID_AMPDU_STATUS = 20,
    ID_VHT = 21,
    ID_TIMESTAMP = 22,
    ID_LAST = 23,
    ID_RADIOTAP_NAMESPACE = 29,
    ID_VENDOR_NAMESPACE = 30,
    ID_EXT = 31
  };

  RadioTapField(const RadioTapField::ID id_ = ID_ERR);

  virtual
  ~RadioTapField();

  virtual bool
  Assemble(zSocket::Buffer& sb_, const uint8_t* hdr_, size_t& pad_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, const uint8_t* hdr_, size_t& pad_);

  uint8_t*
  Assemble(uint8_t* hdr_, uint8_t* frame_, size_t& rem_, size_t& pad_);

  uint8_t*
  Disassemble(uint8_t* hdr_, uint8_t* frame_, size_t& rem_, size_t& pad_);

  RadioTapField::ID
  Id() const;

  size_t
  Align() const;

  size_t
  Size() const;

  size_t
  GetValue(uint8_t* p_, size_t len_) const;

  template<typename T>
    bool
    GetValue(T& value_) const
    {
      bool status = false;
      if (sizeof(T) == this->Size())
      {
        status = (this->GetValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
      }
      return (status);
    }

  size_t
  PutValue(const uint8_t* p_, size_t len_);

  template<typename T>
    bool
    PutValue(T& value_)
    {
      return (this->PutValue((uint8_t*) &value_, sizeof(T)) == sizeof(T));
    }

  virtual void
  Display() const;

protected:

private:

  RadioTapField::ID _id;
  std::vector<uint8_t> _value;

};

//*****************************************************************************
// Class: RadioTapFieldTsft
//*****************************************************************************

class RadioTapFieldTsft : public RadioTapField
{

public:

  RadioTapFieldTsft();

  virtual
  ~RadioTapFieldTsft();

  virtual uint64_t
  operator()() const;

  virtual bool
  operator()(const uint64_t val_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldFlags
//*****************************************************************************

class RadioTapFieldFlags : public RadioTapField
{

public:

  enum FLAGS
  {
    FLAGS_CFP = 0x01,
    FLAGS_SHORTPRE = 0x02,
    FLAGS_WEP = 0x04,
    FLAGS_FRAG = 0x08,
    FLAGS_FCS = 0x10,
    FLAGS_DATAPAD = 0x20,
    FLAGS_BADFCS = 0x40,
    FLAGS_SHORTGI = 0x80
  };

  RadioTapFieldFlags();

  virtual
  ~RadioTapFieldFlags();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  bool
  CFP() const;

  bool
  CFP(const bool flag_);

  bool
  ShortPreamble() const;

  bool
  ShortPreamble(const bool flag_);

  bool
  WEP() const;

  bool
  WEP(const bool flag_);

  bool
  Fragmentation() const;

  bool
  Fragmentation(const bool flag_);

  bool
  FCS() const;

  bool
  FCS(const bool flag_);

  bool
  DataPad() const;

  bool
  DataPad(const bool flag_);

  bool
  BadFCS() const;

  bool
  BadFCS(const bool flag_);

  bool
  ShortGI() const;

  bool
  ShortGI(const bool flag_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldRate
//*****************************************************************************

class RadioTapFieldRate : public RadioTapField
{

public:

  RadioTapFieldRate();

  virtual
  ~RadioTapFieldRate();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  float
  RateBps() const;

  bool
  RateBsp(const float rate_);

  float
  RateKbps() const;

  bool
  RateKbps(const float rate_);

  float
  RateMbps() const;

  bool
  RateMbps(const float rate_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldTxPower
//*****************************************************************************

class RadioTapFieldTxPower : public RadioTapField
{

public:

	RadioTapFieldTxPower();

  virtual
  ~RadioTapFieldTxPower();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  float
  TxPower() const;

  bool
  TxPower(const float txPower_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldMcs
//*****************************************************************************

struct mcs;

class RadioTapFieldMcs : public RadioTapField
{

public:

  enum WIDTH
  {
    WIDTH_ERR = -1,
    WIDTH_NONE = 0,
    WIDTH_20 = 1,
    WIDTH_40 = 2,
    WIDTH_20L = 3,
    WIDTH_20U = 4,
    WIDTH_LAST
  };

  enum GUARD_INT
  {
    GUARD_INT_ERR = -1,
    GUARD_INT_NONE = 0,
    GUARD_INT_LONG = 1,
    GUARD_INT_SHORT = 2,
    GUARD_INT_LAST
  };

  enum HT_FORMAT
  {
    HT_FORMAT_ERR = -1,
    HT_FORMAT_NONE = 0,
    HT_FORMAT_MIXED = 1,
    HT_FORMAT_GREEN = 2,
    HT_FORMAT_LAST
  };

  enum STBC_STREAMS
  {
    STBC_STREAMS_ERR = -1,
    STBC_STREAMS_NONE = 0,
    STBC_STREAMS_ONE = 1,
    STBC_STREAMS_TWO = 2,
    STBC_STREAMS_THREE = 3,
    STBC_STREAMS_LAST
  };

  RadioTapFieldMcs();

  virtual
  ~RadioTapFieldMcs();

  virtual struct mcs
  operator()() const;

  virtual bool
  operator()(const struct mcs val_);

  WIDTH
  Width() const;

  bool
  Width(const WIDTH width_);

  RadioTapFieldMcs::GUARD_INT
  GuardInterval() const;

  bool
  GuardInterval(const RadioTapFieldMcs::GUARD_INT gi_);

  RadioTapFieldMcs::HT_FORMAT
  HtFormat() const;

  bool
  HtFormat(const RadioTapFieldMcs::HT_FORMAT ht_);

  RadioTapFieldMcs::STBC_STREAMS
  Streams() const;

  bool
  Streams(const RadioTapFieldMcs::STBC_STREAMS streams_);

  uint8_t
  Index() const;

  bool
  Index( const uint8_t index_);

  float
  RateBps() const;

  float
  RateKbps() const;

  float
  RateMbps() const;

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldChannel
//*****************************************************************************

class RadioTapFieldChannel : public RadioTapField
{

public:

  enum FLAGS
  {
    FLAGS_TURBO = 0x0010,
    FLAGS_CCK = 0x0020,
    FLAGS_OFDM = 0x0040,
    FLAGS_2GHZ = 0x0080,
    FLAGS_5GHZ = 0x0100,
    FLAGS_PASSIVE = 0x0200,
    FLAGS_DYN = 0x0400,
    FLAGS_GFSK = 0x0800,
    FLAGS_GSM = 0x1000,
    FLAGS_STURBO = 0x2000,
    FLAGS_HALF = 0x4000,
    FLAGS_QUARTER = 0x8000,
  };

  RadioTapFieldChannel();

  virtual
  ~RadioTapFieldChannel();

  virtual uint32_t
  operator()() const;

  virtual bool
  operator()(const uint32_t val_);

  uint16_t
  Frequency() const;

  bool
  Frequency(const uint16_t val_);

  uint16_t
  Channel() const;

  bool
  Channel(const uint16_t val_);

  uint16_t
  Flags() const;

  bool
  Flags(const uint16_t val_);

  bool
  Turbo() const;

  bool
  Turbo (const bool flag_);

  bool
  CCK() const;

  bool
  CCK (const bool flag_);

  bool
  OFDM() const;

  bool
  OFDM (const bool flag_);

  bool
  Spectrum2G() const;

  bool
  Spectrum2G (const bool flag_);

  bool
  Spectrum5G() const;

  bool
  Spectrum5G (const bool flag_);

  bool
  Passive() const;

  bool
  Passive (const bool flag_);

  bool
  DynamicCCK() const;

  bool
  DynamicCCK (const bool flag_);

  bool
  GFSK() const;

  bool
  GFSK (const bool flag_);

  bool
  GSM() const;

  bool
  GSM (const bool flag_);

  bool
  StaticTurbo() const;

  bool
  StaticTurbo (const bool flag_);

  bool
  HalfRate() const;

  bool
  HalfRate (const bool flag_);

  bool
  QuarterRate() const;

  bool
  QuarterRate (const bool flag_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldSignalLevel
//*****************************************************************************

class RadioTapFieldSignalLevel : public RadioTapField
{

public:

  RadioTapFieldSignalLevel();

  virtual
  ~RadioTapFieldSignalLevel();

  virtual int8_t
  operator()() const;

  virtual bool
  operator()(const int8_t val_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldSignalNoise
//*****************************************************************************

class RadioTapFieldSignalNoise : public RadioTapField
{

public:

  RadioTapFieldSignalNoise();

  virtual
  ~RadioTapFieldSignalNoise();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldAntenna
//*****************************************************************************

class RadioTapFieldAntenna : public RadioTapField
{

public:

  RadioTapFieldAntenna();

  virtual
  ~RadioTapFieldAntenna();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldRxFlags
//*****************************************************************************

class RadioTapFieldRxFlags : public RadioTapField
{

public:

  RadioTapFieldRxFlags();

  virtual
  ~RadioTapFieldRxFlags();

  virtual uint16_t
  operator()() const;

  virtual bool
  operator()(const uint16_t val_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldTxFlags
//*****************************************************************************

class RadioTapFieldTxFlags : public RadioTapField
{

public:

  RadioTapFieldTxFlags();

  virtual
  ~RadioTapFieldTxFlags();

  virtual uint16_t
  operator()() const;

  virtual bool
  operator()(const uint16_t val_);

  bool
  Failed() const;

  bool
  Failed(const bool flag_);

  bool
  UseRtsCts() const;

  bool
  UseRtsCts(const bool flag_);

  bool
  NoAck() const;

  bool
  NoAck(const bool flag_);

  bool
  NoSeqNum() const;

  bool
  NoSeqNum(const bool flag_);

  virtual void
  Display() const;

};

//*****************************************************************************
// Class: RadioTapFieldDataRetries
//*****************************************************************************

class RadioTapFieldDataRetries : public RadioTapField
{

public:

  RadioTapFieldDataRetries();

  virtual
  ~RadioTapFieldDataRetries();

  virtual uint8_t
  operator()() const;

  virtual bool
  operator()(const uint8_t val_);

  virtual void
  Display() const;

};

}
}
}

#endif // __IEEE80211_RADIOTAPFIELD_H__
