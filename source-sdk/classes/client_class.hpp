#pragma once
#include "recv_props.hpp"

class client_class;
class i_client_networkable;

typedef i_client_networkable* (*create_client_class_fn)(int ent_number, int serial_number);
typedef i_client_networkable* (*create_event_fn)();

enum class_ids {
	cai_basenpc = 0,
	cak47 = 1,
	cbaseanimating = 2,
	cbaseanimatingoverlay = 3,
	cbaseattributableitem = 4,
	cbasebutton = 5,
	cbasecombatcharacter = 6,
	cbasecombatweapon = 7,
	cbasecsgrenade = 8,
	cbasecsgrenadeprojectile = 9,
	cbasedoor = 10,
	cbaseentity = 11,
	cbaseflex = 12,
	cbasegrenade = 13,
	cbaseparticleentity = 14,
	cbaseplayer = 15,
	cbasepropdoor = 16,
	cbaseteamobjectiveresource = 17,
	cbasetempentity = 18,
	cbasetoggle = 19,
	cbasetrigger = 20,
	cbaseviewmodel = 21,
	cbasevphysicstrigger = 22,
	cbaseweaponworldmodel = 23,
	cbeam = 24,
	cbeamspotlight = 25,
	cbonefollower = 26,
	cbrc4target = 27,
	cbreachcharge = 28,
	cbreachchargeprojectile = 29,
	cbreakableprop = 30,
	cbreakablesurface = 31,
	cc4 = 32,
	ccascadelight = 33,
	cchicken = 34,
	ccolorcorrection = 35,
	ccolorcorrectionvolume = 36,
	ccsgamerulesproxy = 37,
	ccsplayer = 38,
	ccsplayerresource = 39,
	ccsragdoll = 40,
	ccsteam = 41,
	cdangerzone = 42,
	cdangerzonecontroller = 43,
	cdeagle = 44,
	cdecoygrenade = 45,
	cdecoyprojectile = 46,
	cdrone = 47,
	cdronegun = 48,
	cdynamiclight = 49,
	cdynamicprop = 50,
	ceconentity = 51,
	ceconwearable = 52,
	cembers = 53,
	centitydissolve = 54,
	centityflame = 55,
	centityfreezing = 56,
	centityparticletrail = 57,
	cenvambientlight = 58,
	cenvdetailcontroller = 59,
	cenvdofcontroller = 60,
	cenvgascanister = 61,
	cenvparticlescript = 62,
	cenvprojectedtexture = 63,
	cenvquadraticbeam = 64,
	cenvscreeneffect = 65,
	cenvscreenoverlay = 66,
	cenvtonemapcontroller = 67,
	cenvwind = 68,
	cfeplayerdecal = 69,
	cfirecrackerblast = 70,
	cfiresmoke = 71,
	cfiretrail = 72,
	cfish = 73,
	cfists = 74,
	cflashbang = 75,
	cfogcontroller = 76,
	cfootstepcontrol = 77,
	cfunc_dust = 78,
	cfunc_lod = 79,
	cfuncareaportalwindow = 80,
	cfuncbrush = 81,
	cfuncconveyor = 82,
	cfuncladder = 83,
	cfuncmonitor = 84,
	cfuncmovelinear = 85,
	cfuncoccluder = 86,
	cfuncreflectiveglass = 87,
	cfuncrotating = 88,
	cfuncsmokevolume = 89,
	cfunctracktrain = 90,
	cgamerulesproxy = 91,
	cgrassburn = 92,
	chandletest = 93,
	chegrenade = 94,
	chostage = 95,
	chostagecarriableprop = 96,
	cincendiarygrenade = 97,
	cinferno = 98,
	cinfoladderdismount = 99,
	cinfomapregion = 100,
	cinfooverlayaccessor = 101,
	citem_healthshot = 102,
	citemcash = 103,
	citemdogtags = 104,
	cknife = 105,
	cknifegg = 106,
	clightglow = 107,
	cmaterialmodifycontrol = 108,
	cmelee = 109,
	cmolotovgrenade = 110,
	cmolotovprojectile = 111,
	cmoviedisplay = 112,
	cparadropchopper = 113,
	cparticlefire = 114,
	cparticleperformancemonitor = 115,
	cparticlesystem = 116,
	cphysbox = 117,
	cphysboxmultiplayer = 118,
	cphysicsprop = 119,
	cphysicspropmultiplayer = 120,
	cphysmagnet = 121,
	cphyspropammobox = 122,
	cphysproplootcrate = 123,
	cphyspropradarjammer = 124,
	cphyspropweaponupgrade = 125,
	cplantedc4 = 126,
	cplasma = 127,
	cplayerresource = 128,
	cpointcamera = 129,
	cpointcommentarynode = 130,
	cpointworldtext = 131,
	cposecontroller = 132,
	cpostprocesscontroller = 133,
	cprecipitation = 134,
	cprecipitationblocker = 135,
	cpredictedviewmodel = 136,
	cprop_hallucination = 137,
	cpropcounter = 138,
	cpropdoorrotating = 139,
	cpropjeep = 140,
	cpropvehicledriveable = 141,
	cragdollmanager = 142,
	cragdollprop = 143,
	cragdollpropattached = 144,
	cropekeyframe = 145,
	cscar17 = 146,
	csceneentity = 147,
	csensorgrenade = 148,
	csensorgrenadeprojectile = 149,
	cshadowcontrol = 150,
	cslideshowdisplay = 151,
	csmokegrenade = 152,
	csmokegrenadeprojectile = 153,
	csmokestack = 154,
	csnowball = 155,
	csnowballpile = 156,
	csnowballprojectile = 157,
	cspatialentity = 158,
	cspotlightend = 159,
	csprite = 160,
	cspriteoriented = 161,
	cspritetrail = 162,
	cstatueprop = 163,
	csteamjet = 164,
	csun = 165,
	csunlightshadowcontrol = 166,
	csurvivalspawnchopper = 167,
	ctablet = 168,
	cteam = 169,
	cteamplayroundbasedrulesproxy = 170,
	ctearmorricochet = 171,
	ctebasebeam = 172,
	ctebeamentpoint = 173,
	ctebeaments = 174,
	ctebeamfollow = 175,
	ctebeamlaser = 176,
	ctebeampoints = 177,
	ctebeamring = 178,
	ctebeamringpoint = 179,
	ctebeamspline = 180,
	ctebloodsprite = 181,
	ctebloodstream = 182,
	ctebreakmodel = 183,
	ctebspdecal = 184,
	ctebubbles = 185,
	ctebubbletrail = 186,
	cteclientprojectile = 187,
	ctedecal = 188,
	ctedust = 189,
	ctedynamiclight = 190,
	cteeffectdispatch = 191,
	cteenergysplash = 192,
	cteexplosion = 193,
	ctefirebullets = 194,
	ctefizz = 195,
	ctefootprintdecal = 196,
	ctefoundryhelpers = 197,
	ctegaussexplosion = 198,
	cteglowsprite = 199,
	cteimpact = 200,
	ctekillplayerattachments = 201,
	ctelargefunnel = 202,
	ctemetalsparks = 203,
	ctemuzzleflash = 204,
	cteparticlesystem = 205,
	ctephysicsprop = 206,
	cteplantbomb = 207,
	cteplayeranimevent = 208,
	cteplayerdecal = 209,
	cteprojecteddecal = 210,
	cteradioicon = 211,
	cteshattersurface = 212,
	cteshowline = 213,
	ctesla = 214,
	ctesmoke = 215,
	ctesparks = 216,
	ctesprite = 217,
	ctespritespray = 218,
	ctest_proxytoggle_networkable = 219,
	ctesttraceline = 220,
	cteworlddecal = 221,
	ctriggerplayermovement = 222,
	ctriggersoundoperator = 223,
	cvguiscreen = 224,
	cvotecontroller = 225,
	cwaterbullet = 226,
	cwaterlodcontrol = 227,
	cweaponaug = 228,
	cweaponawp = 229,
	cweaponbaseitem = 230,
	cweaponbizon = 231,
	cweaponcsbase = 232,
	cweaponcsbasegun = 233,
	cweaponcycler = 234,
	cweaponelite = 235,
	cweaponfamas = 236,
	cweaponfiveseven = 237,
	cweapong3sg1 = 238,
	cweapongalil = 239,
	cweapongalilar = 240,
	cweaponglock = 241,
	cweaponhkp2000 = 242,
	cweaponm249 = 243,
	cweaponm3 = 244,
	cweaponm4a1 = 245,
	cweaponmac10 = 246,
	cweaponmag7 = 247,
	cweaponmp5navy = 248,
	cweaponmp7 = 249,
	cweaponmp9 = 250,
	cweaponnegev = 251,
	cweaponnova = 252,
	cweaponp228 = 253,
	cweaponp250 = 254,
	cweaponp90 = 255,
	cweaponsawedoff = 256,
	cweaponscar20 = 257,
	cweaponscout = 258,
	cweaponsg550 = 259,
	cweaponsg552 = 260,
	cweaponsg556 = 261,
	cweaponssg08 = 262,
	cweapontaser = 263,
	cweapontec9 = 264,
	cweapontmp = 265,
	cweaponump45 = 266,
	cweaponusp = 267,
	cweaponxm1014 = 268,
	cworld = 269,
	cworldvguitext = 270,
	dusttrail = 271,
	movieexplosion = 272,
	particlesmokegrenade = 273,
	rockettrail = 274,
	smoketrail = 275,
	sporeexplosion = 276,
};
class c_client_class {
public:
	create_client_class_fn create_fn;
	create_event_fn create_event_fn;
	char* network_name;
	recv_table* recvtable_ptr;
	c_client_class* next_ptr;
	class_ids class_id;
};