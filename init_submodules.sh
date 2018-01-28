git submodule update --init
cd lib/glew-2.1.0
git checkout 3a8eff77da3658c13fbd3634c943d5251d76322c
cd ../..

git submodule update --init
cd lib/glm
git checkout 6fa203eeb7fbcbb6f620501fad40359c8a456049
cd ../..

cd lib/glfw
git checkout 999f3556fdd80983b10051746264489f2cb1ef16
cd ../..

cd lib/DevIL
git checkout e34284a7e07763769f671a74b4fec718174ad862
cd ../..

cd lib/soil
git checkout 4fff135429c4c7996791c019e6cc20a1e9fde568
cd ../..

cd lib/assimp
git checkout 80799bdbf90ce626475635815ee18537718a05b1
cd ../..

cd lib/openexr
git checkout v2.2.1
cd ../..

cd lib/freeglut
git checkout FG_3_0_0
cd ../..

cd lib/freetype-gl
git checkout 507cc68511465981d32257268fee184b79cc69f7
cd ../..

cd lib/assimp
git checkout v4.1.0
cd ../..

cd lib/argh
git checkout v1.2.0
cd ../..
