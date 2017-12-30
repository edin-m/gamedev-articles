git submodule update --init
cd lib/glew
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
