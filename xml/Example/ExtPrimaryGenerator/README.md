##How to use ExtPrimaryGenerator.

* Generate external asc file. The file should like below. First line means particle number in one
 event. Follow with each particle info in format :`1 Particle_ID 0 0 px py pz 0 time 0 0 0 0 0`


    6
    1  9802004  0  0  0.8557E-01  0.1711E+00  -.7148E-01  0.3727E+01  0.00000E+00  # 222Rn alpha decay   5590.300
    1  9802004  0  0  0.2018E-01  -.1949E+00  0.8509E-01  0.3727E+01  0.48240E+12  # 218Po alpha decay   6114.700
    1       11  0  0  0.2639E-03  0.4570E-03  0.1025E-03  0.5110E-03  0.30764E+13  # 214Pb beta decay     230.709
    1       22  0  0  0.1800E-03  0.1971E-03  -.2293E-03  0.0000E+00  0.00000E+00  # 214Pb beta decay     351.900
    1       11  0  0  0.5546E-03  0.5014E-03  -.6623E-03  0.5110E-03  0.15077E+13  # 214Bi beta decay     610.988
    1  9802004  0  0  -.4865E-01  0.6431E-02  -.2368E+00  0.3727E+01  0.21949E+06  # 214Po alpha decay   7833.500



* Add ExtPrimaryGenerator in xml file and set path to asc file.


      <generator name="ExtPrimaryGenerator">
        <parameter name="InputFile" value="/root/herbertqiao/GIT/BambooMC/xml/Example/ExtPrimaryGenerator/Un1000000Sd1.asc"/>