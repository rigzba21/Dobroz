package main

import (
	"log"

	"Dobroz/dobroz/actions"
)

func main() {
	app := actions.App()
	if err := app.Serve(); err != nil {
		log.Fatal(err)
	}
}
