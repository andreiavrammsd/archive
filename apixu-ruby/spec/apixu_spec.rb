require 'spec_helper'

describe Apixu::Client do
  it 'has a version number' do
    expect(Apixu::VERSION).not_to be nil
  end

  it 'allows creating new clients' do
    expect(Apixu::Client.new('key')).to be_an_instance_of(Apixu::Client)
  end

  def client
    Apixu::Client.new ENV['APIXUKEY']
  end

  it 'raises an error with empty API key' do
    expect do
      Apixu::Client.new ''
    end.to raise_error(Apixu::Errors::InvalidKey)
  end

  it 'retrieves conditions list' do
    result = client.conditions
    expect(result).to match_json_schema('conditions')
  end

  it 'retrieves current weather by query' do
    result = client.current 'Paris'
    expect(result).to match_json_schema('current')
  end

  it 'retrieves weather forecasts by query and number of days (and optional hour)' do
    result = client.forecast 'Paris', 3, 12
    expect(result).to match_json_schema('forecast')
  end

  it 'retrieves historical weather information by query and date (and optional until date)' do
    result = client.history 'Paris', Date.today
    expect(result).to match_json_schema('history')
  end

  it 'raises error if history date is invalid' do
    expect do
      client.history 'Paris', ''
    end.to raise_error(ArgumentError)

    expect do
      client.history 'Paris', Date.today, ''
    end.to raise_error(ArgumentError)
  end

  it 'it searches for locations' do
    result = client.search 'London'
    expect(result).to match_json_schema('search')
  end

  it 'raises error when query is empty' do
    expect do
      client.search ''
    end.to raise_error(Apixu::Errors::Request)
  end
end
