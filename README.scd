// ca4sc: cellular automata for supercollider
// ugens and classes

// ======= installation destination

in SuperCollider run:

(
// 1. create folder
if(File.exists(Platform.userExtensionDir).not, {
	File.mkdir(Platform.userExtensionDir);
});
)

// 2. open folder
Platform.userExtensionDir.openOS;

// 3. place the downloaded folder there

// 4. recompile class library (SuperCollider>Menu>Language)

// ugen
[CA1]
[CA2]
[CA1x]
[CA2x]

// pattern
[Pca1]
[Pca2]

// TODO
explore ca2

// references
"hacking cellular automata: an approach to sound synthesis" (hacking.pdf)
scsymposium.rtfd

// contact
yota morimoto
yotamorimoto [at] gmail.com