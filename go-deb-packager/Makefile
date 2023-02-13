RELEASE=release/${PACKAGE}_${VERSION}

export GOROOT := /usr/local/go
export GOPATH := /go
export PATH := $(GOPATH)/bin:$(GOROOT)/bin:$(PATH)

all: init build package clean

init:
	@cp -r template/package ${RELEASE}
	@find ${RELEASE} -name ".gitkeep" -delete

	@mv ${RELEASE}/etc/init.d/script ${RELEASE}/etc/init.d/${PACKAGE}
	@chmod 0755 ${RELEASE}/etc/init.d/${PACKAGE}
	@sed -i -E "s#(<PACKAGE>)#${PACKAGE}#" ${RELEASE}/etc/init.d/${PACKAGE}
	@sed -i -E "s#(<SHORT_DESCRIPTION>)#${SHORT_DESCRIPTION}#" ${RELEASE}/etc/init.d/${PACKAGE}
	@sed -i -E "s#(<DESCRIPTION>)#${DESCRIPTION}#" ${RELEASE}/etc/init.d/${PACKAGE}
	@sed -i -E "s#(<DAEMON>)#/usr/local/bin/${PACKAGE}#" ${RELEASE}/etc/init.d/${PACKAGE}

	@mv ${RELEASE}/etc/package ${RELEASE}/etc/${PACKAGE}
	@cp -r config/* ${RELEASE}/etc/${PACKAGE}
	
	@chmod 0755 ${RELEASE}/DEBIAN/postinst
	@sed -i -E "s#(<PACKAGE>)#${PACKAGE}#" ${RELEASE}/DEBIAN/postinst

	@sed -i -E "s#(<PACKAGE>)#${PACKAGE}#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<VERSION>)#${VERSION}#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<SECTION>)#${SECTION}#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<PRIORITY>)#${PRIORITY}#" ${RELEASE}/DEBIAN/control
	@ARCHITECTURE="amd64"; sed -i -E "s#(<ARCHITECTURE>)#$$ARCHITECTURE#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<MAINTAINER>)#${MAINTAINER}#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<SHORT_DESCRIPTION>)#${SHORT_DESCRIPTION}#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<DESCRIPTION>)#${DESCRIPTION}#" ${RELEASE}/DEBIAN/control

build:
	@cd /go/src/${PACKAGE}; go get -t; go test; go build ${LDFLAGS} -race -o /pkg/${RELEASE}/usr/local/bin/${PACKAGE}

	@chmod +x ${RELEASE}/usr/local/bin/${PACKAGE}
	@SIZE=`du -s ${RELEASE} | cut -d "r" -f 1`; \
			sed -i -E "s#(<SIZE>)#$$SIZE#" ${RELEASE}/DEBIAN/control

	@CHECKSUM_HASH=`shasum -a 1 ${RELEASE}/usr/local/bin/${PACKAGE} | cut -d ' ' -f 1`; \
		sed -i -E "s#(<CHECKSUM_HASH>)#$$CHECKSUM_HASH#" ${RELEASE}/DEBIAN/control
	@sed -i -E "s#(<CHECKSUM_ALGORITHM>)#SHA1#" ${RELEASE}/DEBIAN/control

package:
	dpkg-deb --build ${RELEASE}

clean:
	@rm -rf ${RELEASE}
	@cd /go/src/${PACKAGE}; go clean 
