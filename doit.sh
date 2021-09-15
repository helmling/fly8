conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"  
conan remote update bincrafters  https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
conan config set general.revisions_enabled=1
conan install conanfile.txt 

#cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1
