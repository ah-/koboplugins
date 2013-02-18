mkdir package
pushd package
mkdir -p usr/local/Kobo
touch usr/local/Kobo/libpoc.so
touch usr/local/Kobo/libtest.so
touch usr/local/Kobo/libtweaks.so
mkdir -p usr/local/Trolltech/QtEmbedded-4.6.2-arm/plugins/imageformats/
cp ../libtweaks.so usr/local/Trolltech/QtEmbedded-4.6.2-arm/plugins/imageformats/
tar cvzf KoboRoot.tgz usr
zip tweaks-`date +%y-%m-%d` KoboRoot.tgz
popd
