rm -r src/*
rm -r build/*
mkdir src
mkdir build
asn1c Registration.asn1 -D src -no-gen-OER -no-gen-example
gcc -Isrc -o build/coder client_coder.cpp src/*.c -DPDU=Registration -DASN_DISABLE_OER_SUPPORT
gcc -Isrc -o build/decoder client_decoder.cpp src/*.c -DPDU=Registration -DASN_DISABLE_OER_SUPPORT
./build/coder ./build/Registration_ber_out
./build/decoder ./build/Registration_ber_out
