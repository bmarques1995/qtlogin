const validServices = new Map<string, number>();

const servicesEnum = {
    github: 1,
    google: 2
};

validServices.set("github", servicesEnum.github);
validServices.set("google", servicesEnum.google);

export {servicesEnum, validServices};