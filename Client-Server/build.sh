rm -r src/*
rm -r build/*
mkdir src
mkdir build
asn1c Registration.asn1 -D src -no-gen-OER -no-gen-example
gcc -Isrc -o build/coder Registration_coder.c src/*.c -DPDU=Rectangle -DASN_DISABLE_OER_SUPPORT
gcc -Isrc -o build/decoder Registration_decoder.c src/*.c -DPDU=Rectangle -DASN_DISABLE_OER_SUPPORT
./build/coder ./build/Registration_ber_out
./build/decoder ./build/Registration_ber_out
