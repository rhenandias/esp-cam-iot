import React, { useState, useEffect } from "react";
import firebase from "../../services/firebase";
import api from "../../services/api";

import {
  Image,
  Button,
  Divider,
  Center,
  Icon,
  Flex,
  Box,
  Heading,
  Spacer,
  Link,
} from "@chakra-ui/react";

import { RepeatIcon } from "@chakra-ui/icons";

import { DiGithubBadge } from "react-icons/di";

import { GoMarkGithub } from "react-icons/go";

export default function Home() {
  const [picture, setpicture] = useState();
  const [updated, setUpdated] = useState(false);

  useEffect(() => {
    function updatePicture() {
      const ref = firebase.database().ref("camera//live");
      ref.on("child_added", (snapshot) => {
        const { base64data } = snapshot.val();
        setpicture((picture) => base64data);
        setUpdated(false);
      });
    }
    updatePicture();
  }, []);

  async function newpicture() {
    setUpdated(true);
    api
      .post(`/setflag?path=updates&value=1`)
      .then((res) => {
        console.log(res.data);
      })
      .catch((error) => {
        console.log(error.message);
      });
  }

  return (
    <>
      <Flex py="6" px="16" alignItems="center">
        <Flex>
          <Heading size="lg">ESP32 Cam IOT</Heading>
        </Flex>
        <Spacer />
        <Link href="https://github.com/rhenandias/esp-cam-iot" isExternal>
          <GoMarkGithub size="28px" />
        </Link>
        <Box></Box>
      </Flex>
      <Box px="8">
        <Divider />
      </Box>

      <Center m={4}>
        <Image
          rounded="lg"
          height="400px"
          src={`data:image/jpeg;base64,${picture}`}
        />
      </Center>
      <Center>
        <Button
          isLoading={updated}
          onClick={newpicture}
          leftIcon={<RepeatIcon />}
          colorScheme="teal"
          variant="outline"
        >
          Nova Imagem
        </Button>
      </Center>
    </>
  );
}
