from .nd import NoiseDistribution, stddevf
from .lwe_parameters import LWEParameters
TiGER128 = LWEParameters(
    n=512,
    q=256,
# RLWR estimate 
    Xs=NoiseDistribution.SparseTernary(512,52,52),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 is smaller than RLWE. so, estimate this.

# RLWE estimate     
#    Xs=NoiseDistribution.SparseTernary(512,52,52), #RLWE
#    Xe=NoiseDistribution.DiscreteGaussian(0.559), # RLWE e_1=HWT(512,32)=stdev 0.25 + c_u=Uniform[0,1]=stdev 0.5. then e_1+c+u = "0.559"
    m=512,
    tag="TiGER128",
)

TiGER192 = LWEParameters(
    n=1024,
    q=256,
# RLWR estimate
    Xs=NoiseDistribution.SparseTernary(1024,58,58),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 is smaller than RLWE. so, estimate this.

# RLWE estimate
#    Xs=NoiseDistribution.SparseTernary(1024,58,58),
#    Xe=NoiseDistribution.DiscreteGaussian(0.53), # RLWE e_1=HWT(1024,32)=stdev 0.18, c_u=Uniform[0,1]=stdev 0.5. then e_1+c_u = "0.53"
    m=1024,
    tag="TiGER192",
)

TiGER256 = LWEParameters(
    n=1024,
    q=256,
# RLWR estimate
    Xs=NoiseDistribution.SparseTernary(1024,92,92),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 =RLWE stdev. so, estimate this.

# RLWE estimate   
#    Xs=NoiseDistribution.SparseTernary(1024,92,92), 
#    Xe=NoiseDistribution.DiscreteGaussian(0.5), # RLWE e_1=HWT(1024,256)=stdev 0.5 + c_u=0. then e_1 + c_u = "0.5"
    m=1024,
    tag="TiGER256",
)
