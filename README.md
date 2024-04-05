[![Robot Hardware](https://github.com/Kitsune-Robotics/KIT-Controller/actions/workflows/hardware_workflow.yml/badge.svg)](https://github.com/Kitsune-Robotics/KIT-Controller/actions/workflows/hardware_workflow.yml)

# KIT-Controller

Hardware for the KIT Controller

Migrated from https://kitsunehosting.net/gitea/Kenwood/lewis-crawler


# Getting Started

## Clone this repo

```shell
git clone https://github.com/Kitsune-Robotics/KIT-Controller.git --recurse-submodules
```

You will *need* the submodules as all of the boards in `Hardware` have several library dependencies.

`Hardware/BackPlane` Contains the backplane for the system, the backplane connects all other cards together.
`Hardware/KIT_Controller` Is the main processing unit of the system, the core computer where this code (will eventually) live
`Hardware/Dev_Card` Is a development tool, made to be slotted into any slot and connected via probes, solder jumpers and wires to aid in developing/debugging


# Parts

The kit controller and its other boards use the Sullins [EBM15DSEH](https://www.digikey.com/en/products/detail/sullins-connector-solutions/EBM15DSEH/927297) to connect to the backplane.

Its a little expensive, in the future the [WingTAT ED30MEBGFBK ](https://www.lcsc.com/product-detail/Card-Edge-Connectors_WingTAT-ED30MEBGFBK_C5242012.html) is a possible replacement.
