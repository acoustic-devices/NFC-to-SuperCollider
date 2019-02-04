/*JEU DE SOUS-VERRES - VERSION RYTHMIQUE
COMPOSITION ET CREATION PAR THOMAS DESAI
CREE AVEC NFC, OSC, ET SCD*/


/* CONCEPT -
NFC SUR UN SOUSVERRE
METTRE SOUSVERRES SUR CAPTEURS
ENVOYER OSC AVEC ARDUINO VERS ORDI
CONTROLER SC
*/


// SERVER BOOT

s.boot;

// RAW SOUNDS

//wabam
x=(
{
	var root, scale, s;
	// whistle
	var trig, p, exc, x, freq;
		root = rrand (3,8);
		scale = #[0,2,4,5,7,9,11];
			freq = (scale.choose + #[84,96].choose + root).midicps;
			exc = BrownNoise.ar([0.05,0.05]) * max(0, SinOsc.kr(exprand(0.125,0.5), 2pi.rand));
			s = (Klank.ar(`[
					Array[freq],
					nil,
					Array[0.3]
				], exc) * 0.1).softclip;
}
);


//new-wave
y=(
{

		Pan2.ar(
	RLPF.ar(LFPulse.ar(exprand(50,600), 2pi.rand, 0.5,

		Mix.ar(SinOsc.ar(exprand(0.3,8) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).max(0)
		* Mix.ar(SinOsc.ar(exprand(6,24) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand])).abs
	), exprand(100,2000), 0.2), 1.0.rand2);
}
);

//clang
z=(
{

	// metallic
	var trig, p, exc, x, s;
	exc = BrownNoise.ar(Decay2.kr(Impulse.kr(#[0.125,0.25,0.375,0.5,0.75,1,1.5,2].choose,0,[0.04,0.04]), 0.005, 0.05));
		s = (Klank.ar(`[Array.fill(rrand (4,16), {linrand(8000.0)+60}),nil,Array.fill(16, {rrand(0.1,2.0)})], exc) * 0.1).softclip;
}
);




// extra variable for Pbinds

c = [0, 2, 4, 5, 7, 9]; //harmonic variables


// COMPOSITION/PBINDS

~player1 = {
		Pbind(x.play, Prand(c), \dur, 0.1).asStream};

~player2 = {
		Pbind(y.play, Prand(c), \dur, 0.1).asStream};


~player3 = {
		Pbind(z.play, Prand(c), \dur, 0.1).asStream};


//OSC CONTROLS

// TO PLAY SOUNDS
(OSCdef.new(\wabam, {
	arg msg,time,addr,port;
	msg.postln;
	if(
		msg[1] == 1,
		{o =(~player1.play)});
},'/oscControl/grid/1'));


(OSCdef.new(\neuwave, {
	arg msg,time,addr,port;
	msg.postln;
	if(
		msg[1] == 1,
		{o =(~player2.play)});
},'/oscControl/grid/2'));


(OSCdef.new(\metaldrum, {
	arg msg,time,addr,port;
	msg.postln;
	if(
		msg[1] == 1,
		{o =(~player3.play)});
},'/oscControl/grid/3'));


// TO STOP SOUNDS
(OSCdef.new(\Silence, {
	arg msg,time,addr,port;
	msg.postln;
	if(
		msg[1] == 1,
		{m =(s.freeAll)});
},'/oscControl/grid/4'));


//TEST PLAY + STOP

//~player.play; // plays the sound -- here as test in case no OSC input -- commented out as default


//s.freeAll // stops the sound -- here as kill in case of no OSC input --  commented out as default////





