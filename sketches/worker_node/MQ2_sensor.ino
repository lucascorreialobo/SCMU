
int get_MQ2_values(){

  int gasValue = analogRead(MQ2_PIN);

  Serial.print("MQ2 sensor AO value: ");
  Serial.println(gasValue);
	return gasValue;
}