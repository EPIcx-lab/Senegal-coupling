# Mobility Resolution Needed to Inform Predictive Epidemic Models for Spatial Transmission from Mobile Phone Data

## Authors

- **Giulia Pullano** – Department of Biology, Georgetown University, Washington, DC, USA
- **Shweta Bansal** – Department of Biology, Georgetown University, Washington, DC, USA
- **Stefania Rubrichi** – Orange Research – SENSE, Châtillon, France
- **Vittoria Colizza** – Sorbonne Université, INSERM, Institut Pierre Louis d'Epidémiologie et de Santé Publique, Paris, France

## Overview
This repository contains the datasets supporting the paper: [Mobility resolution needed to inform predictive epidemic models for spatial transmission from mobile phone data](https://www.medrxiv.org/content/10.1101/2024.10.11.24315335).


The study analyzes how different definitions and aggregation levels of human mobility extracted from mobile phone data affect the spatial spread of infectious diseases. Using anonymized mobility trajectories from Orange mobile phone users in Senegal, three epidemiological coupling definitions are compared:

D: Displacement-based (high resolution)

L: Location-based (medium resolution)

C: Most visited location-based (low resolution)

These couplings are integrated into a stochastic SEIR metapopulation model to assess epidemic diffusion under varying mobility resolutions.


## Data Description
### 1. Mobility Matrices (matrices/)

Each file contains origin–destination pairs of municipalities and the estimated mobility flow (epidemiological coupling) for a given month and coupling definition.


### 2. Shapefile (shapefile_Senegal/)

Geographic boundaries of Senegalese municipalities used to map mobility matrices and charcaterize Urban/Rural areas.

###3. Population Data (population_2013.csv)

Census data from the 2013 Senegal population census, used for model initialization.

## Citation
If you use these data, please cite as:

Pullano, G., Bansal, S., Rubrichi, S., & Colizza, V. (2025). Mobility Resolution Needed to Inform Predictive Epidemic Models for Spatial Transmission from Mobile Phone Data. medRxiv, 2024.10.11.24315335 (2024).
