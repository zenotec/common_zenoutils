/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#ifndef __NL80211_PHYBANDSATTRIBUTE_H__
#define __NL80211_PHYBANDSATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes
#include <map>

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

static uint16_t
_pfreq2chan(uint16_t freq_)
{

  uint16_t channel = 0;
  // Channels 1 - 14
  if ((freq_ >= 2412) && (freq_ <= 2477))
  {
    channel = (1 + ((freq_ - 2412) / 5));
  }
  // Channels 36 - 64
  else if ((freq_ >= 5170) && (freq_ <= 5320))
  {
    channel = (34 + ((freq_ - 5170) / 5));
  }
  // Channels 100 - 144
  else if ((freq_ >= 5500) && (freq_ <= 5720))
  {
    channel = (100 + ((freq_ - 5500) / 5));
  }
  // Channels 149 - 161
  else if ((freq_ >= 5745) && (freq_ <= 5805))
  {
    channel = (149 + ((freq_ - 5745) / 5));
  }
  // Channel 165
  else if (freq_ == 5825)
  {
    channel = 165;
  }

  return (channel);

}

static uint16_t
_pchan2freq(uint16_t chan_)
{
  uint16_t freq = 0;
  if ((chan_ >= 1) && (chan_ <=13))
  {
    freq = (((chan_ - 1) * 5) + 2412);
  }
  else if ((chan_ >= 36) && (chan_ <= 64))
  {
    freq = (((chan_ - 34) * 5) + 5170);
  }
  else if ((chan_ >= 100) && (chan_ <= 144))
  {
    freq = (((chan_ - 100) * 5) + 5500);
  }
  else if ((chan_ >= 149) && (chan_ <= 161))
  {
    freq = (((chan_ - 149) * 5) + 5745);
  }
  else if (chan_ == 165)
  {
    freq = 5825;
  }
  return (freq);
}

// ,
//,
//__NL80211_FREQUENCY_ATTR_NO_IBSS,
//NL80211_FREQUENCY_ATTR_RADAR,
//NL80211_FREQUENCY_ATTR_MAX_TX_POWER,
//NL80211_FREQUENCY_ATTR_DFS_STATE,
//NL80211_FREQUENCY_ATTR_DFS_TIME,
//NL80211_FREQUENCY_ATTR_NO_HT40_MINUS,
//NL80211_FREQUENCY_ATTR_NO_HT40_PLUS,
//NL80211_FREQUENCY_ATTR_NO_80MHZ,
//NL80211_FREQUENCY_ATTR_NO_160MHZ,
//NL80211_FREQUENCY_ATTR_DFS_CAC_TIME,
//NL80211_FREQUENCY_ATTR_INDOOR_ONLY,
//NL80211_FREQUENCY_ATTR_IR_CONCURRENT,
//NL80211_FREQUENCY_ATTR_NO_20MHZ,
//NL80211_FREQUENCY_ATTR_NO_10MHZ,

//*****************************************************************************
// Class: PhyBandsFreqAttributeFreq
//*****************************************************************************

class PhyBandsFreqAttributeFreq :
    public AttributeValue
{

public:

  PhyBandsFreqAttributeFreq() :
    AttributeValue(NL80211_FREQUENCY_ATTR_FREQ)
  {
  }

  virtual
  ~PhyBandsFreqAttributeFreq()
  {
  }

  uint32_t
  operator()() const
  {
    return (this->Get(uint32_t(0)));
  }

  bool
  operator()(const uint32_t freq_)
  {
    return (this->Set(freq_));
  }

  uint32_t
  GetChannel() const
  {
    return(_pfreq2chan(this->operator()()));
  }

  bool
  SetChannel(const uint32_t channel_)
  {
    return(this->operator()(_pchan2freq(channel_)));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Frequency: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t[" << this->GetChannel() << "]: " << this->operator ()() << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsFreqAttributeDisabled
//*****************************************************************************

class PhyBandsFreqAttributeDisabled :
    public AttributeValue
{

public:

  PhyBandsFreqAttributeDisabled() :
    AttributeValue(NL80211_FREQUENCY_ATTR_DISABLED)
  {
  }

  virtual
  ~PhyBandsFreqAttributeDisabled()
  {
  }

  bool
  operator()() const
  {
    return (this->IsValid());
  }

  bool
  operator()(const uint32_t flag_)
  {
    if (flag_)
    {
      this->SetValid();
    }
    else
    {
      this->ClrValid();
    }
    return (true);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Disabled: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t" << std::string(this->IsValid() ? "true" : "false") << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsFreqAttributeNoIR
//*****************************************************************************

class PhyBandsFreqAttributeNoIR :
    public AttributeValue
{

public:

  PhyBandsFreqAttributeNoIR() :
    AttributeValue(NL80211_FREQUENCY_ATTR_NO_IR)
  {
  }

  virtual
  ~PhyBandsFreqAttributeNoIR()
  {
  }

  bool
  operator()() const
  {
    return (this->IsValid());
  }

  bool
  operator()(const uint32_t flag_)
  {
    if (flag_)
    {
      this->SetValid();
    }
    else
    {
      this->ClrValid();
    }
    return (true);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "No IR: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t" << std::string(this->IsValid() ? "true" : "false") << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsFreqAttributeRadar
//*****************************************************************************

class PhyBandsFreqAttributeRadar :
    public AttributeValue
{

public:

  PhyBandsFreqAttributeRadar() :
    AttributeValue(NL80211_FREQUENCY_ATTR_RADAR)
  {
  }

  virtual
  ~PhyBandsFreqAttributeRadar()
  {
  }

  bool
  operator()() const
  {
    return (this->IsValid());
  }

  bool
  operator()(const uint32_t flag_)
  {
    if (flag_)
    {
      this->SetValid();
    }
    else
    {
      this->ClrValid();
    }
    return (true);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Radar Detect: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t" << std::string(this->IsValid() ? "true" : "false") << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsFreqAttributeMaxPower
//*****************************************************************************

class PhyBandsFreqAttributeMaxPower :
    public AttributeValue
{

public:

  PhyBandsFreqAttributeMaxPower() :
    AttributeValue(NL80211_FREQUENCY_ATTR_MAX_TX_POWER)
  {
  }

  virtual
  ~PhyBandsFreqAttributeMaxPower()
  {
  }

  uint32_t
  operator()() const
  {
    return (this->Get(uint32_t(0)));
  }

  bool
  operator()(const uint32_t power_)
  {
    return (this->Set(power_));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Max TX Power: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t" << this->operator ()() << " dBm" << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsFreqAttribute
//*****************************************************************************

class PhyBandsFreqAttribute :
    public AttributeNested
{

public:

  PhyBandsFreqAttribute(const int id_) :
    AttributeNested(id_)
  {
  }

  virtual
  ~PhyBandsFreqAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    AttributeNested::operator =(other_);
    this->Get(&this->_freq);
    this->Get(&this->_disabled);
    this->Get(&this->_noir);
    this->Get(&this->_radar);
    this->Get(&this->_power);
    return (*this);
  }

  virtual size_t
  GetLength() const
  {
    size_t len = 0;
    if (this->_freq.IsValid())
      len += sizeof(struct nlattr) + this->_freq.GetLength();
    if (this->_disabled.IsValid())
      len += sizeof(struct nlattr) + this->_disabled.GetLength();
    if (this->_noir.IsValid())
      len += sizeof(struct nlattr) + this->_noir.GetLength();
    if (this->_radar.IsValid())
      len += sizeof(struct nlattr) + this->_radar.GetLength();
    if (this->_power.IsValid())
      len += sizeof(struct nlattr) + this->_power.GetLength();
    return (len);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Frequency: " << this->GetLength() << std::endl;
    if (this->_freq.IsValid())
      this->_freq.Display(std::string(prefix_ + "\t"));
    if (this->_disabled.IsValid())
      this->_disabled.Display(std::string(prefix_ + "\t"));
    if (this->_noir.IsValid())
      this->_noir.Display(std::string(prefix_ + "\t"));
    if (this->_radar.IsValid())
      this->_radar.Display(std::string(prefix_ + "\t"));
    if (this->_power.IsValid())
      this->_power.Display(std::string(prefix_ + "\t"));
  }

protected:

private:

  PhyBandsFreqAttributeFreq _freq;
  PhyBandsFreqAttributeDisabled _disabled;
  PhyBandsFreqAttributeNoIR _noir;
  PhyBandsFreqAttributeRadar _radar;
  PhyBandsFreqAttributeMaxPower _power;

};

//*****************************************************************************
// Class: PhyBandsFreqsAttribute
//*****************************************************************************

class PhyBandsFreqsAttribute :
    public AttributeNested
{

public:

  PhyBandsFreqsAttribute() :
    AttributeNested(NL80211_BAND_ATTR_FREQS)
  {
  }

  virtual
  ~PhyBandsFreqsAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    int cnt = 0;
    this->_freqs.clear();
    AttributeNested::operator =(other_);
    for (int cnt = 0; cnt < 16; cnt++)
    {
      this->_freqs.emplace_back(cnt);
      if (!this->Get(&this->_freqs[cnt]))
      {
        this->_freqs.erase(this->_freqs.end());
        break;
      }
    }
    return (*this);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Frequencies: " << this->GetLength() << std::endl;
    FOREACH (auto& freq, this->_freqs)
    {
      freq.Display(std::string(prefix_ + "\t"));
    }
  }

protected:

private:

  std::vector<PhyBandsFreqAttribute> _freqs;

};

//*****************************************************************************
// Class: PhyBandsRateAttributeRate
//*****************************************************************************

class PhyBandsRateAttributeRate :
    public AttributeValue
{

public:

  PhyBandsRateAttributeRate() :
    AttributeValue(NL80211_BITRATE_ATTR_RATE)
  {
  }

  virtual
  ~PhyBandsRateAttributeRate()
  {
  }

  uint32_t
  operator()() const
  {
    return (this->Get(uint32_t(0)));
  }

  bool
  operator()(const uint32_t rate_)
  {
    return (this->Set(rate_));
  }

  uint32_t
  GetKbps() const
  {
    return (this->operator ()() * 100);
  }

  bool
  SetKbps(const uint32_t rate_)
  {
    return (this->operator ()(rate_ / 100));
  }

  uint32_t
  GetMbps() const
  {
    return (this->operator ()() / 10);
  }

  bool
  SetMbps(const uint32_t rate_)
  {
    return (this->operator ()(rate_ * 10));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Rate: " << this->GetLength() << std::endl;
    std::cout << prefix_ << "\t" << this->GetKbps() << " Kbps" << std::endl;
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsRateAttribute
//*****************************************************************************

class PhyBandsRateAttribute :
    public AttributeNested
{

public:

  PhyBandsRateAttribute(const int id_) :
    AttributeNested(id_)
  {
  }

  virtual
  ~PhyBandsRateAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    AttributeNested::operator =(other_);
    this->Get(&this->_rate);
    return (*this);
  }

  virtual size_t
  GetLength() const
  {
    size_t len = 0;
    if (this->_rate.IsValid())
      len += sizeof(struct nlattr) + this->_rate.GetLength();
    return (len);
  }

  uint8_t
  GetRate() const
  {
    return (this->_rate.GetKbps() / 500); // convert units to 500 Kbps
  }

  bool
  SetRate(const uint8_t rate_)
  {
    return (this->_rate.SetKbps(rate_ * 500)); // convert units from 500 Kbps
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Rate: " << this->GetLength() << std::endl;
    if (this->_rate.IsValid())
      this->_rate.Display(std::string(prefix_ + "\t"));
  }

protected:

private:

  PhyBandsRateAttributeRate _rate;

};

//*****************************************************************************
// Class: PhyBandsRatesAttribute
//*****************************************************************************

class PhyBandsRatesAttribute :
    public AttributeNested
{

public:

  PhyBandsRatesAttribute() :
    AttributeNested(NL80211_BAND_ATTR_RATES)
  {
  }

  virtual
  ~PhyBandsRatesAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    int cnt = 0;
    this->_rates.clear();
    AttributeNested::operator =(other_);
    for (int cnt = 0; cnt < 16; cnt++)
    {
      this->_rates.emplace_back(cnt);
      if (!this->Get(&this->_rates[cnt]))
      {
        this->_rates.erase(this->_rates.end());
        break;
      }
    }
    return (*this);
  }

  std::vector<uint8_t>
  operator()() const
  {
    std::vector<uint8_t> rates;
    FOREACH(auto& rate, this->_rates)
    {
      rates.push_back(rate.GetRate());
    }
    return (rates);
  }

  bool
  operator()(const std::vector<uint8_t>& rates_)
  {
    this->_rates.clear();
    for (int rate = 0; rate < rates_.size(); rate++)
    {
      this->_rates.emplace_back(rate);
      this->_rates[rate].SetRate(rates_[rate]);
    }
    return (!this->_rates.empty());
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Rates: " << this->GetLength() << std::endl;
    FOREACH (auto& rate, this->_rates)
    {
      rate.Display(std::string(prefix_ + "\t"));
    }
  }

protected:

private:

  std::vector<PhyBandsRateAttribute> _rates;

};

//*****************************************************************************
// Class: PhyBandsHtMcsAttribute
//*****************************************************************************

class PhyBandsHtMcsAttribute :
    public AttributeValue
{

public:

  struct tx_mcs
  {
    uint8_t tx_bits;
    uint16_t reserved1;
    uint8_t reserved2;
  } __attribute__ ((packed));

  struct mcs_set
  {
    std::array<uint8_t,10> rx_mcs_bitmask;
    uint16_t rx_highest_rate;
    tx_mcs tx_mcs_fields;
  } __attribute__ ((packed));

  PhyBandsHtMcsAttribute() :
    AttributeValue(NL80211_BAND_ATTR_HT_MCS_SET)
  {
  }

  virtual
  ~PhyBandsHtMcsAttribute()
  {
  }

  struct mcs_set
  operator()() const
  {
    struct mcs_set mcs = { 0 };
    return (this->Get(mcs));
  }

  bool
  operator()(const struct mcs_set& rate_)
  {
    return (this->Set(rate_));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "HT MCS Set: " << this->GetLength() << std::endl;
    AttributeValue::Display(prefix_);
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsHtCapabiltiesAttribute
//*****************************************************************************

class PhyBandsHtCapabiltiesAttribute :
    public AttributeValue
{

public:

  PhyBandsHtCapabiltiesAttribute() :
    AttributeValue(NL80211_BAND_ATTR_HT_CAPA)
  {
  }

  virtual
  ~PhyBandsHtCapabiltiesAttribute()
  {
  }

  uint16_t
  operator()() const
  {
    return (this->Get(uint16_t(0)));
  }

  bool
  operator()(const uint16_t caps_)
  {
    return (this->Set(caps_));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "HT Capabilities: " << this->GetLength() << std::endl;
    printf("%s0x%04x\n", prefix_.c_str(), this->operator ()());
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsAmpduFactorAttribute
//*****************************************************************************

class PhyBandsAmpduFactorAttribute :
    public AttributeValue
{

public:

  PhyBandsAmpduFactorAttribute() :
    AttributeValue(NL80211_BAND_ATTR_HT_AMPDU_FACTOR)
  {
  }

  virtual
  ~PhyBandsAmpduFactorAttribute()
  {
  }

  uint8_t
  operator()() const
  {
    return (this->Get(uint8_t(0)));
  }

  bool
  operator()(const uint8_t ampdu_)
  {
    return (this->Set(ampdu_));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "A-MPDU Factor: " << this->GetLength() << std::endl;
    printf("%s0x%02x\n", prefix_.c_str(), this->operator ()());
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandsAmpduDensityAttribute
//*****************************************************************************

class PhyBandsAmpduDensityAttribute :
    public AttributeValue
{

public:

  PhyBandsAmpduDensityAttribute() :
    AttributeValue(NL80211_BAND_ATTR_HT_AMPDU_DENSITY)
  {
  }

  virtual
  ~PhyBandsAmpduDensityAttribute()
  {
  }

  uint8_t
  operator()() const
  {
    return (this->Get(uint8_t(0)));
  }

  bool
  operator()(const uint8_t ampdu_)
  {
    return (this->Set(ampdu_));
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "A-MPDU Density: " << this->GetLength() << std::endl;
    printf("%s0x%02x\n", prefix_.c_str(), this->operator ()());
  }

protected:

private:

};

//*****************************************************************************
// Class: PhyBandAttribute
//*****************************************************************************
/**
 * enum nl80211_band_attr - band attributes
 * @__NL80211_BAND_ATTR_INVALID: attribute number 0 is reserved
 * @NL80211_BAND_ATTR_FREQS: supported frequencies in this band,
 *  an array of nested frequency attributes
 * @NL80211_BAND_ATTR_RATES: supported bitrates in this band,
 *  an array of nested bitrate attributes
 * @NL80211_BAND_ATTR_HT_MCS_SET: 16-byte attribute containing the MCS set as
 *  defined in 802.11n
 * @NL80211_BAND_ATTR_HT_CAPA: HT capabilities, as in the HT information IE
 * @NL80211_BAND_ATTR_HT_AMPDU_FACTOR: A-MPDU factor, as in 11n
 * @NL80211_BAND_ATTR_HT_AMPDU_DENSITY: A-MPDU density, as in 11n
 * @NL80211_BAND_ATTR_VHT_MCS_SET: 32-byte attribute containing the MCS set as
 *  defined in 802.11ac
 * @NL80211_BAND_ATTR_VHT_CAPA: VHT capabilities, as in the HT information IE
 * @NL80211_BAND_ATTR_MAX: highest band attribute currently defined
 * @__NL80211_BAND_ATTR_AFTER_LAST: internal use
 */
class PhyBandAttribute :
    public AttributeNested
{

public:

  PhyBandAttribute(const int band_ = 0) :
    AttributeNested(band_)
  {
  }

  virtual
  ~PhyBandAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    AttributeNested::operator =(other_);
    this->Get(&this->_freqs);
    this->Get(&this->_rates);
    this->Get(&this->_htmcs);
    this->Get(&this->_htcaps);
    this->Get(&this->_ampdu_factor);
    this->Get(&this->_ampdu_density);
    return (*this);
  }

  const PhyBandsFreqsAttribute&
  GetChannels() const
  {
    return (this->_freqs);
  }

  bool
  SetChannels(const PhyBandsFreqsAttribute& attr_)
  {
    this->_freqs = attr_;
    return (true);
  }

  const PhyBandsRatesAttribute&
  GetRates () const
  {
    return (this->_rates);
  }

  bool
  SetRates(const PhyBandsRatesAttribute& attr_)
  {
    this->_rates = attr_;
    return (true);
  }

  const PhyBandsHtMcsAttribute&
  GetMcsSet () const
  {
    return (this->_htmcs);
  }

  bool
  SetMcsSet(const PhyBandsHtMcsAttribute& attr_)
  {
    this->_htmcs = attr_;
    return (true);
  }

  const PhyBandsHtCapabiltiesAttribute&
  GetHtCaps () const
  {
    return (this->_htcaps);
  }

  bool
  SetHtCaps(const PhyBandsHtCapabiltiesAttribute& attr_)
  {
    this->_htcaps = attr_;
    return (true);
  }

  const PhyBandsAmpduFactorAttribute&
  GetAmpduFactor () const
  {
    return (this->_ampdu_factor);
  }

  bool
  SetAmpduFactor(const PhyBandsAmpduFactorAttribute& attr_)
  {
    this->_ampdu_factor = attr_;
    return (true);
  }

  const PhyBandsAmpduDensityAttribute&
  GetAmpduDensity () const
  {
    return (this->_ampdu_density);
  }

  bool
  SetAmpduDensity(const PhyBandsAmpduDensityAttribute& attr_)
  {
    this->_ampdu_density = attr_;
    return (true);
  }

  virtual size_t
  GetLength() const
  {
    size_t len = 0;
    if (this->_freqs.IsValid())
      len += sizeof(struct nlattr) + this->_freqs.GetLength();
    if (this->_rates.IsValid())
      len += sizeof(struct nlattr) + this->_rates.GetLength();
    if (this->_htmcs.IsValid())
      len += sizeof(struct nlattr) + this->_htmcs.GetLength();
    if (this->_htcaps.IsValid())
      len += sizeof(struct nlattr) + this->_htcaps.GetLength();
    if (this->_ampdu_factor.IsValid())
      len += sizeof(struct nlattr) + this->_ampdu_factor.GetLength();
    if (this->_ampdu_density.IsValid())
      len += sizeof(struct nlattr) + this->_ampdu_density.GetLength();
    return (len);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "Band[" << this->GetId() << "]: " << this->GetLength() << std::endl;
    if (this->_freqs.IsValid())
      this->_freqs.Display(std::string(prefix_ + "\t"));
    if (this->_rates.IsValid())
      this->_rates.Display(std::string(prefix_ + "\t"));
    if (this->_htmcs.IsValid())
      this->_htmcs.Display(std::string(prefix_ + "\t"));
    if (this->_htcaps.IsValid())
      this->_htcaps.Display(std::string(prefix_ + "\t"));
    if (this->_ampdu_factor.IsValid())
      this->_ampdu_factor.Display(std::string(prefix_ + "\t"));
    if (this->_ampdu_density.IsValid())
      this->_ampdu_density.Display(std::string(prefix_ + "\t"));
  }

protected:

private:

  PhyBandsFreqsAttribute _freqs;
  PhyBandsRatesAttribute _rates;
  PhyBandsHtMcsAttribute _htmcs;
  PhyBandsHtCapabiltiesAttribute _htcaps;
  PhyBandsAmpduFactorAttribute _ampdu_factor;
  PhyBandsAmpduDensityAttribute _ampdu_density;
};

//*****************************************************************************
// Class: PhyBandsAttribute
//*****************************************************************************

class PhyBandsAttribute :
    public AttributeNested
{

public:

  PhyBandsAttribute() :
    AttributeNested(NL80211_ATTR_WIPHY_BANDS)
  {
  }

  virtual
  ~PhyBandsAttribute()
  {
  }

  virtual AttributeNested&
  operator=(const Attribute* other_)
  {
    AttributeNested::operator =(other_);
    for (int band = 0; band <= NL80211_BAND_60GHZ; band++)
    {
      this->_bands.emplace(band, band);
      if (this->Get(&this->_bands[band]))
      {
        this->_bands[band].SetId(band);
      }
      else
      {
        this->_bands.erase(band);
      }
    }
    return (*this);
  }

  std::vector<uint8_t>
  GetBands() const
  {
    std::vector<uint8_t> bands;
    FOREACH(auto& band, this->_bands)
    {
      bands.push_back(band.first);
    }
    return (bands);
  }

  PhyBandAttribute
  GetPhyBand(const int id_)
  {
    PhyBandAttribute band;
    if (this->_bands.count(id_) > 0)
    {
      band = this->_bands.at(id_);
    }
    return (band);
  }

  bool
  SetPhyBand(const int id_, const PhyBandAttribute& band_)
  {
    bool status = false;
    if (id_ <= NL80211_BAND_60GHZ)
    {
      this->_bands[id_] = band_;
      status = true;
    }
    return (status);
  }

  virtual void
  Display(const std::string& prefix_ = "") const
  {
    std::cout << prefix_ << "PhyBands:" << std::endl;
    FOREACH(auto& band, this->_bands)
    {
      band.second.Display(std::string(prefix_ + "\t"));
    }
  }

protected:

private:

  std::map<int, PhyBandAttribute> _bands;

};

}

#endif /* __NL80211_PHYBANDSATTRIBUTE_H__ */
